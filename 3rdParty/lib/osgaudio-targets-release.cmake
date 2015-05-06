#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "osgAudio" for configuration "Release"
set_property(TARGET osgAudio APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(osgAudio PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/osgAudio.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "C:\Program Files (x86)\FMOD SoundSystem\EX\FMOD Programmers API Windows\api\lib\fmodexL64_vc.lib;C:/3DEngine/OpenSceneGraph/install/lib/osg.lib;C:/3DEngine/OpenSceneGraph/install/lib/osgDB.lib;C:/3DEngine/OpenSceneGraph/install/lib/osgUtil.lib;C:/3DEngine/OpenSceneGraph/install/lib/osgGA.lib;C:/3DEngine/OpenSceneGraph/install/lib/osgText.lib;C:/3DEngine/OpenSceneGraph/install/lib/osgViewer.lib;C:/3DEngine/OpenSceneGraph/install/lib/OpenThreads.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/osgAudio.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS osgAudio )
list(APPEND _IMPORT_CHECK_FILES_FOR_osgAudio "${_IMPORT_PREFIX}/lib/osgAudio.lib" "${_IMPORT_PREFIX}/bin/osgAudio.dll" )

# Import target "osgdb_osgAudio" for configuration "Release"
set_property(TARGET osgdb_osgAudio APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(osgdb_osgAudio PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/osgdb_osgAudio.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS osgdb_osgAudio )
list(APPEND _IMPORT_CHECK_FILES_FOR_osgdb_osgAudio "${_IMPORT_PREFIX}/bin/osgdb_osgAudio.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
