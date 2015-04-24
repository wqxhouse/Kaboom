#include "stdafx.h" 

#include <osg/io_utils>
#include <osg/PolygonMode>
#include <osg/Geometry>
#include <osg/View>
#include <osgUtil/CullVisitor>
#include <iostream>
#include <sstream>
#include "EffectCompositor.h"
#include "Core.h"

using namespace osgFX;

/* PassCullCallback */
void PassCullCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	osg::Camera* camera = static_cast<osg::Camera*>(node);
	if (!camera || !nv)
	{
		traverse(node, nv);
		return;
	}

	if (nv->getVisitorType() == osg::NodeVisitor::CULL_VISITOR)
	{
		osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>(nv);
		if (_type == EffectCompositor::FORWARD_PASS)
		{
			// Forward pass will traverse the scene normally
			_compositor->osg::Group::traverse(*nv);

			// mainly used for skybox
			if (!_preserveNearFar) return;

			// We obtain the actual near/far values at the end of forward pass traversing
			double znear = cv->getCalculatedNearPlane();
			double zfar = cv->getCalculatedFarPlane();
			osg::Matrixd projection = *(cv->getProjectionMatrix());
			cv->clampProjectionMatrix(projection, znear, zfar);

			const osg::FrameStamp* fs = cv->getFrameStamp();
			// if ( fs ) _compositor->setPreservedNearAndFar( fs->getFrameNumber(), znear, zfar );
			if (fs) _compositor->setPreservedNearAndFarAndClampMat(fs->getFrameNumber(), znear, zfar, projection);
		}
		else if (camera->getNumChildren() > 0)  // Use camera's own children as display surface
			camera->osg::Group::traverse(*nv);
		else                                    // Render to a fullscreen quad
			_compositor->getOrCreateQuad()->accept(*nv);
	}
	else
		traverse(node, nv);
}

/* EffectCompositor */

EffectCompositor::EffectCompositor()
:   _renderTargetResolution(1024.0f, 1024.0f, 1.0f),
    _renderTargetImpl(osg::Camera::FRAME_BUFFER_OBJECT),
    _preservedZNear(FLT_MAX), _preservedZFar(-FLT_MAX),
    _preservingNearFarFrameNumber(0),
	_isDirty(false) 
{
    getOrCreateQuad();
    setCurrentTechnique( "default" );
    clearPassList();  // just create an empty pass list for "default"
}

EffectCompositor::EffectCompositor( const EffectCompositor& copy, const osg::CopyOp& copyop )
:   osg::Group(copy, copyop),
    _passLists(copy._passLists), _textureMap(copy._textureMap),
    _uniformMap(copy._uniformMap), _shaderMap(copy._shaderMap),
	_uniformBufferMap(copy._uniformBufferMap),
    _inbuiltUniforms(copy._inbuiltUniforms),
    _currentTechnique(copy._currentTechnique), _quad(copy._quad),
    _renderTargetResolution(copy._renderTargetResolution),
    _renderTargetImpl(copy._renderTargetImpl),
    _preservedZNear(copy._preservedZNear),
    _preservedZFar(copy._preservedZFar),
    _preservingNearFarFrameNumber(copy._preservingNearFarFrameNumber), 
	_preservedClampedProjectionMatrix(copy._preservedClampedProjectionMatrix),
	_isDirty(copy._isDirty) 
{
}

osg::Camera* EffectCompositor::createNewPass( PassType type, const std::string& name )
{
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setName( name );
    camera->setClearMask( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
    camera->setRenderOrder( osg::Camera::PRE_RENDER );
    camera->setRenderTargetImplementation( _renderTargetImpl );
    camera->setCullCallback( new PassCullCallback(this, type) );
    
    if ( type==DEFERRED_PASS )
    {
        // Deferred pass is absolutely facing the XOY plane in the range of [0, 1]
        camera->setAllowEventFocus( false );
        camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
        camera->setProjectionMatrix( osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0) );
        camera->setViewMatrix( osg::Matrix::identity() );
        camera->addChild( createScreenQuad(1.0, 1.0) );
    }
    else
    {
        // Forward pass will follow the scene navigation result
        camera->setReferenceFrame( osg::Transform::RELATIVE_RF );
    }
    
    PassData newData;
    newData.activated = true;
    newData.type = type;
    newData.name = name;
    newData.pass = camera;
    
    getPassList().push_back( newData );
    return camera.get();
}

bool EffectCompositor::removePass( const std::string& name )
{
    PassList& passList = getPassList();
    for ( unsigned int i=0; i<passList.size(); ++i )
    {
        if ( passList[i].name==name )
        {
            passList.erase( getPassList().begin()+i );
            return true;
        }
    }
    return false;
}

bool EffectCompositor::getPassData( const std::string& name, PassData& data ) const
{
    const PassList& passList = getPassList();
    for ( unsigned int i=0; i<passList.size(); ++i )
    {
        const PassData& pd = passList[i];
        if ( pd.name==name )
        {
            data = pd;
            return true;
        }
    }
    return false;
}

bool EffectCompositor::setPassIndex( const std::string& name, unsigned int index )
{
    PassData passToInsert;
    unsigned int insertIndex = index;
    if ( insertIndex>=getPassList().size() ) return false;
    
    PassList& passList = getPassList();
    for ( unsigned int i=0; i<passList.size(); ++i )
    {
        const PassData& pd = passList[i];
        if ( pd.name==name )
        {
            if ( i!=insertIndex )
            {
                passToInsert = pd;
                passList.erase( getPassList().begin()+i );
                if ( i<insertIndex ) insertIndex--;
            }
            break;
        }
    }
    
    if ( passToInsert.pass.valid() )
    {
        passList.insert( passList.begin()+insertIndex, passToInsert );
        return true;
    }
    return false;
}

unsigned int EffectCompositor::getPassIndex( const std::string& name ) const
{
    const PassList& passList = getPassList();
    for ( unsigned int i=0; i<passList.size(); ++i )
    {
        if ( passList[i].name==name )
            return i;
    }
    return passList.size();
}

bool EffectCompositor::setPassActivated( const std::string& name, bool activated )
{
    PassList& passList = getPassList();
    for ( unsigned int i=0; i<passList.size(); ++i )
    {
        PassData& pd = passList[i];
        if ( pd.name==name )
        {
            pd.activated = activated;
            return true;
        }
    }
    return false;
}

bool EffectCompositor::getPassActivated( const std::string& name ) const
{
    const PassList& passList = getPassList();
    for ( unsigned int i=0; i<passList.size(); ++i )
    {
        if ( passList[i].name==name )
            return passList[i].activated;
    }
    return false;
}

const EffectCompositor::PassList& EffectCompositor::getPassList() const
{
    PassListMap::const_iterator itr = _passLists.find( _currentTechnique );
    if ( itr==_passLists.end() )
    {
        static PassList s_emptyPassList;
        OSG_NOTICE << "Not a valid technique name: " << _currentTechnique << std::endl;
        return s_emptyPassList;
    }
    return itr->second;
}

osg::NodeList EffectCompositor::getCameras( EffectCompositor::PassType type ) const
{
    osg::NodeList cameras;
    const PassList& passList = getPassList();
    for ( unsigned int i=0; i<passList.size(); ++i )
    {
        if ( passList[i].type==type )
            cameras.push_back( passList[i].pass );
    }
    return cameras;
}

bool EffectCompositor::setTexture( const std::string& name, osg::Texture* tex )
{
    TextureMap::iterator itr = _textureMap.find(name);
    if ( itr!=_textureMap.end() )
    {
        itr->second = tex;
        return false;
    }
    else
    {
        _textureMap[name] = tex;
        return true;
    }
}

bool EffectCompositor::removeTexture( const std::string& name )
{
    TextureMap::iterator itr = _textureMap.find(name);
    if ( itr==_textureMap.end() ) return false;
    
    _textureMap.erase( itr );
    return true;
}

osg::Texture* EffectCompositor::getTexture( const std::string& name )
{
    TextureMap::iterator itr = _textureMap.find(name);
    if ( itr==_textureMap.end() ) return NULL;
    else return itr->second.get();
}

const osg::Texture* EffectCompositor::getTexture( const std::string& name ) const
{
    TextureMap::const_iterator itr = _textureMap.find(name);
    if ( itr==_textureMap.end() ) return NULL;
    else return itr->second.get();
}

bool EffectCompositor::setUniform( const std::string& name, osg::Uniform* uniform )
{
    UniformMap::iterator itr = _uniformMap.find(name);
    if ( itr!=_uniformMap.end() )
    {
        itr->second = uniform;
        return false;
    }
    else
    {
        _uniformMap[name] = uniform;
        return true;
    }
}

bool EffectCompositor::removeUniform( const std::string& name )
{
    UniformMap::iterator itr = _uniformMap.find(name);
    if ( itr==_uniformMap.end() ) return false;
    
    _uniformMap.erase( itr );
    return true;
}

osg::Uniform* EffectCompositor::getUniform( const std::string& name )
{
    UniformMap::iterator itr = _uniformMap.find(name);
    if ( itr==_uniformMap.end() ) return NULL;
    else return itr->second.get();
}

const osg::Uniform* EffectCompositor::getUniform( const std::string& name ) const
{
    UniformMap::const_iterator itr = _uniformMap.find(name);
    if ( itr==_uniformMap.end() ) return NULL;
    else return itr->second.get();
}

void EffectCompositor::cacheUniformFloatBuffer(osg::FloatArray *fbuffer)
{
	_uniformFloatBuffer.push_back(fbuffer);
}

bool EffectCompositor::setUniformBufferBinding(const std::string &name, osg::UniformBufferBinding *ubb, osg::FloatArray *fbuffer)
{
	UniformBufferMap::iterator itr = _uniformBufferMap.find(name);
	if (itr != _uniformBufferMap.end())
	{
		itr->second = ubb;
		return false; // return false for updating the cache... weird
	}
	else
	{
		_uniformBufferMap[name] = ubb;
		cacheUniformFloatBuffer(fbuffer);

		return true;
	}
}

osg::UniformBufferBinding *EffectCompositor::getUniformBufferBinding(const std::string &name)
{
	UniformBufferMap::iterator itr = _uniformBufferMap.find(name);
	if (itr == _uniformBufferMap.end()) return NULL;
	else return itr->second.get();
}

const osg::UniformBufferBinding *EffectCompositor::getUniformBufferBinding(const std::string &name) const
{
	UniformBufferMap::const_iterator itr = _uniformBufferMap.find(name);
	if (itr == _uniformBufferMap.end()) return NULL;
	else return itr->second.get();
}

void EffectCompositor::removeInbuiltUniform( InbuiltUniformType t )
{
    for ( InbuiltUniformList::iterator itr=_inbuiltUniforms.begin(); itr!=_inbuiltUniforms.end(); )
    {
        if ( itr->first==t )
        {
            InbuiltUniformList::iterator oldItr = itr;
            itr++; _inbuiltUniforms.erase( oldItr );
        }
        else ++itr;
    }
}

void EffectCompositor::removeInbuiltUniform( osg::Uniform* u )
{
    for ( InbuiltUniformList::iterator itr=_inbuiltUniforms.begin(); itr!=_inbuiltUniforms.end(); )
    {
        if ( itr->second==u )
        {
            InbuiltUniformList::iterator oldItr = itr;
            itr++; _inbuiltUniforms.erase( oldItr );
        }
        else ++itr;
    }
}

bool EffectCompositor::isInbuiltUniform( osg::Uniform* u ) const
{
    for ( InbuiltUniformList::const_iterator itr=_inbuiltUniforms.begin();
          itr!=_inbuiltUniforms.end(); ++itr )
    {
        if ( itr->second==u ) return true;
    }
    return false;
}

bool EffectCompositor::setShader( const std::string& name, osg::Shader* shader )
{
    ShaderMap::iterator itr = _shaderMap.find(name);
    if ( itr!=_shaderMap.end() )
    {
        itr->second = shader;
        return false;
    }
    else
    {
        _shaderMap[name] = shader;
        return true;
    }
}

bool EffectCompositor::removeShader( const std::string& name )
{
    ShaderMap::iterator itr = _shaderMap.find(name);
    if ( itr==_shaderMap.end() ) return false;
    
    _shaderMap.erase( itr );
    return true;
}

osg::Shader* EffectCompositor::getShader( const std::string& name )
{
    ShaderMap::const_iterator itr = _shaderMap.find(name);
    if ( itr==_shaderMap.end() ) return NULL;
    else return itr->second.get();
}

const osg::Shader* EffectCompositor::getShader( const std::string& name ) const
{
    ShaderMap::const_iterator itr = _shaderMap.find(name);
    if ( itr==_shaderMap.end() ) return NULL;
    else return itr->second.get();
}

osg::Geode* EffectCompositor::getOrCreateQuad()
{
    if ( !_quad )
    {
        osg::Geometry* geom = osg::createTexturedQuadGeometry(
            osg::Vec3(), osg::Vec3(1.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 1.0f, 0.0f) );
        _quad = new osg::Geode;
        _quad->addDrawable( geom );
        
        osg::Array* texcoords = geom->getTexCoordArray(0);
        for ( unsigned int i=1; i<8; ++i )
        {
            geom->setTexCoordArray( i, texcoords );
        }
        
        int values = osg::StateAttribute::OFF|osg::StateAttribute::PROTECTED;
        _quad->getOrCreateStateSet()->setAttribute(
            new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL), values );
        _quad->getOrCreateStateSet()->setMode( GL_LIGHTING, values );
    }
    return _quad.get();
}

void EffectCompositor::setPreservedNearAndFarAndClampMat(unsigned int frame, double zn, double zf, const osg::Matrix &clampedProjMat)
{
	setPreservedNearAndFar(frame, zn, zf);

	// FIXME: might not be ideal or intended to put here
	_preservedClampedProjectionMatrix = clampedProjMat;
}

void EffectCompositor::setPreservedNearAndFar( unsigned int frame, double zn, double zf )
{
    if ( _preservingNearFarFrameNumber!=frame )
    {
        _preservingNearFarFrameNumber = frame;
        _preservedZNear = zn;
        _preservedZFar = zf;
    }
    else
    {
        _preservedZNear = osg::maximum(zn, _preservedZNear);
        _preservedZFar = osg::minimum(zf, _preservedZFar);
    }
}

void EffectCompositor::traverse( osg::NodeVisitor& nv )
{
    if ( nv.getVisitorType()==osg::NodeVisitor::CULL_VISITOR )
    {
		// TODO: make sure with Wang Rui that put traverseAllPasses here is appropriate
		// But this solves off-by-one-frame issue
		traverseAllPasses(nv);

        osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>( &nv );
        osg::RefMatrix* projectionMatrix = cv->getProjectionMatrix();

		// FIXME: make it not so hacky and coupling
		// mainCamera
		// FIXME: currently, I have not figure out why in the 
		// game mode, even though I disabled the camera manipulator
		// and update the viewer->getCamera() using external value
		// the cullvisitor still gives back the old one, overriding the update
		// figure out later
		if (!Core::isInGameMode())
		{
			Core::_cam._projMatrix = *cv->getProjectionMatrix();
			// note that here model view matrix *under this cullvisitor* is actually view matrix
			Core::_cam._viewMatrix = *cv->getModelViewMatrix();
			Core::_cam._eye = cv->getEyeLocal();
			Core::_cam._lookat = cv->getLookVectorLocal();
			Core::_cam._up = cv->getUpLocal();
		}

		double fovy = 0.0, aspectRatio = 0.0, zNear = 0.0, zFar = 0.0;
		if (projectionMatrix) projectionMatrix->getPerspective(fovy, aspectRatio, zNear, zFar);
		if (_preservedZNear != FLT_MAX) zNear = _preservedZNear;
		if (_preservedZFar != -FLT_MAX) zFar = _preservedZFar;

		Core::_cam._screenSize = Core::getScreenSize();

		if (!Core::isInGameMode())
		{
			// a complete hack
			if (!Core::allowChangeEditorProjection())
			{
				Core::_cam._clampedProjMatrix = _preservedClampedProjectionMatrix;
				Core::_cam._fovy = fovy;
				Core::_cam._nearPlane = zNear;
				Core::_cam._farPlane = zFar;

				Core::_cam._clampedViewProjMatrix = Core::_cam._viewMatrix * Core::_cam._clampedProjMatrix;
			}
		}

        if ( _inbuiltUniforms.size()>0 )
        {
			/*double fovy = 0.0, aspectRatio = 0.0, zNear = 0.0, zFar = 0.0;
			if ( projectionMatrix ) projectionMatrix->getPerspective( fovy, aspectRatio, zNear, zFar );
			if ( _preservedZNear!=FLT_MAX ) zNear = _preservedZNear;
			if ( _preservedZFar!=-FLT_MAX ) zFar = _preservedZFar;
			*/

            for ( InbuiltUniformList::const_iterator itr=_inbuiltUniforms.begin();
                  itr!=_inbuiltUniforms.end(); ++itr )
            {
                if ( !itr->second ) continue;
                switch ( itr->first )
                {
                case EYE_POSITION:
                    itr->second->set( cv->getEyeLocal() );
                    break;
                case VIEW_POINT:
                    itr->second->set( cv->getViewPointLocal() );
                    break;
                case LOOK_VECTOR:
                    itr->second->set( cv->getLookVectorLocal() );
                    break;
                case UP_VECTOR:
                    itr->second->set( cv->getUpLocal() );
                    break;
                case LEFT_VECTOR:
                    itr->second->set( cv->getLookVectorLocal() ^ cv->getUpLocal() );
                    break;
                case VIEWPORT_X:
                    if ( cv->getViewport() ) itr->second->set( (float)cv->getViewport()->x() );
                    break;
                case VIEWPORT_Y:
                    if ( cv->getViewport() ) itr->second->set( (float)cv->getViewport()->y() );
                    break;
                case VIEWPORT_WIDTH:
                    if ( cv->getViewport() ) itr->second->set( (float)cv->getViewport()->width() );
                    break;
                case VIEWPORT_HEIGHT:
                    if ( cv->getViewport() ) itr->second->set( (float)cv->getViewport()->height() );
                    break;
                case WINDOW_MATRIX:
                    itr->second->set( osg::Matrixf(cv->getWindowMatrix()) );
                    break;
                case INV_WINDOW_MATRIX:
                    itr->second->set( osg::Matrixf::inverse(cv->getWindowMatrix()) );
                    break;
                case FRUSTUM_NEAR_PLANE:
                    itr->second->set( (float)zNear );
                    break;
                case FRUSTUM_FAR_PLANE:
                    itr->second->set( (float)zFar );
                    break;
                case SCENE_FOV_IN_RADIANS:
                    itr->second->set( (float)osg::DegreesToRadians(fovy) );
                    break;
                case SCENE_ASPECT_RATIO:
                    itr->second->set( (float)aspectRatio );
                    break;
                case SCENE_MODELVIEW_MATRIX:
                    if ( cv->getModelViewMatrix() ) itr->second->set( osg::Matrixf(*cv->getModelViewMatrix()) );
                    break;
                case SCENE_INV_MODELVIEW_MATRIX:
                    if ( cv->getModelViewMatrix() ) itr->second->set( osg::Matrixf::inverse(*cv->getModelViewMatrix()) );
                    break;
                case SCENE_PROJECTION_MATRIX:
                    if ( projectionMatrix ) itr->second->set( osg::Matrixf(*projectionMatrix) );
                    break;
                case SCENE_INV_PROJECTION_MATRIX:
                    if ( projectionMatrix ) itr->second->set( osg::Matrixf::inverse(*projectionMatrix) );
                    break;
                default: break;
                }
            }
        }
		// TODO: make sure put it here is buggy. Currently moved before assigning built in uniforms
        // traverseAllPasses( nv );
        return;  // don't traverse as usual
    }
    
    if ( nv.getVisitorType()==osg::NodeVisitor::UPDATE_VISITOR ||
         nv.getVisitorType()==osg::NodeVisitor::EVENT_VISITOR )
    {
        traverseAllPasses( nv );  // just handle uniform callbacks
    }
    osg::Group::traverse( nv );
}

osg::Geode* EffectCompositor::createScreenQuad( float width, float height, float scale )
{
    osg::Geometry* geom = osg::createTexturedQuadGeometry(
        osg::Vec3(), osg::Vec3(width,0.0f,0.0f), osg::Vec3(0.0f,height,0.0f),
        0.0f, 0.0f, width*scale, height*scale );
    osg::ref_ptr<osg::Geode> quad = new osg::Geode;
    quad->addDrawable( geom );
    
    int values = osg::StateAttribute::OFF|osg::StateAttribute::PROTECTED;
    quad->getOrCreateStateSet()->setAttribute(
        new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL), values );
    quad->getOrCreateStateSet()->setMode( GL_LIGHTING, values );
    return quad.release();
}
