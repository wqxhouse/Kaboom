#include <Windows.h>

#include <osgViewer/Viewer>

int main() {
    osgViewer::Viewer viewer;

    viewer.realize();

    while (!viewer.done()) {
        viewer.frame();
    }
}
