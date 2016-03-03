#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>

// g++ -o prog cube.cpp -losg -losgDB -losgViewer

int main(void)
{
    osgViewer::Viewer   viewer;
    osg::Group* scene = new osg::Group;
    osg::DisplaySettings::instance()->setNumMultiSamples( 4 );

    viewer.setUpViewInWindow( 100, 50, 800, 600 );
    osgViewer::Viewer::Windows fenetres;
    viewer.getWindows(fenetres);
    fenetres[0]->setWindowName("Test sur OpenSceneGraph");
    viewer.getCamera()->setClearColor( osg::Vec4( 1,1,1,1 ) );

    // Création d'une boîte centrée à l'origine, de dimensions 2x3x4:
    osg::Box* boite = new osg::Box(osg::Vec3(0, 0, 0), 2,3,4);
    osg::ShapeDrawable* boiteDrawable = new osg::ShapeDrawable(boite);
    osg::Geode* geodeCercle = new osg::Geode();
    geodeCercle->addDrawable(boiteDrawable);

    // Création d'une sphère centrée à l'origine, de rayon 1:
    osg::Sphere* sphere = new osg::Sphere( osg::Vec3(0,15,0), 1.0);
    osg::ShapeDrawable* sphereDrawable =  new osg::ShapeDrawable(sphere);
    osg::Geode* geodeSphere = new osg::Geode();
    geodeSphere->addDrawable(sphereDrawable);

    osg::Cone* cone = new osg::Cone(osg::Vec3(0,-15,0), 1.0, 3.0);
    osg::ShapeDrawable* coneDrawable = new osg::ShapeDrawable(cone);
    osg::Geode* geodeCone = new osg::Geode();
    geodeCone->addDrawable(coneDrawable);

    scene->addChild(geodeSphere);
    scene->addChild(geodeCercle);
    scene->addChild(geodeCone);

    viewer.setSceneData(scene);
    return viewer.run();
}
