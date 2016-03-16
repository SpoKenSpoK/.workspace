#include <iostream>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>
#include <osgGA/NodeTrackerManipulator>
#include <osg/LightSource>
#include <osgGA/GUIEventHandler>
#include <osgViewer/ViewerEventHandlers>

using namespace std;

// g++ -o prog cube.cpp -losg -losgDB -losgViewer

class GestionEvenements : public osgGA::GUIEventHandler
{
    public:
    GestionEvenements(osg::ref_ptr<osg::Group> _scene){ scene = _scene ; }
    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
    osg::ref_ptr<osg::Group> scene;
    bool light1;
    bool light2;
};

bool GestionEvenements::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    switch(ea.getEventType())
    {
    case osgGA::GUIEventAdapter::KEYDOWN : // appui sur une touche
        switch(ea.getKey())
        {
            case osgGA::GUIEventAdapter::KEY_Left :
            // On réagit à l'appui sur la flèche gauche
            break ;
            case '1' :
                if(light1){
                    scene->getOrCreateStateSet()->setMode(GL_LIGHT1, osg::StateAttribute::OFF);
                    light1 = false;
                }
                else{
                    scene->getOrCreateStateSet()->setMode(GL_LIGHT1, osg::StateAttribute::ON);
                    light1 = true;
                }
            break ;
            case '2' :
                if(light2){
                    scene->getOrCreateStateSet()->setMode(GL_LIGHT2, osg::StateAttribute::OFF);
                    light2 = false;
                }
                else{
                    scene->getOrCreateStateSet()->setMode(GL_LIGHT2, osg::StateAttribute::ON);
                    light2 = true;
                }
            break ;
        }
    break;

    case osgGA::GUIEventAdapter::PUSH :
    // clic souris
    {
        // coordonnées écran de la souris au moment du clic
        int x = ea.getX();
        int y = ea.getY();
        if( ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
        cout << "bouton gauche" << endl;
        if (ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON)
        cout << "bouton milieu" << endl;
        if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
        cout << "bouton droit" << endl;
        break;
    }
    case osgGA::GUIEventAdapter::DOUBLECLICK :
    // double clic à la souris
    break ;
    }
    return false ;
    // pour que l'événement soit traité par d'autres gestionnaires
}


int main(void)
{
    osgViewer::Viewer   viewer;
    osg::Group* scene = new osg::Group;
    osg::DisplaySettings::instance()->setNumMultiSamples( 4 );
    viewer.addEventHandler(new osgViewer::StatsHandler);

    viewer.setUpViewInWindow( 100, 50, 800, 600 );
    osgViewer::Viewer::Windows fenetres;
    viewer.getWindows(fenetres);
    fenetres[0]->setWindowName("Test sur OpenSceneGraph");
    viewer.getCamera()->setClearColor( osg::Vec4( 0,0.5,1,1 ) );

    osg::ref_ptr<GestionEvenements> gestionnaire = new GestionEvenements(scene);
    viewer.addEventHandler(gestionnaire.get());


    osg::ref_ptr<osg::LightSource> lightOne = new osg::LightSource;
    lightOne->getLight()->setLightNum(1); // GL_LIGHT1
    lightOne->getLight()->setPosition(osg::Vec4(1,-1,1,0)); // 0 = directionnel
    lightOne->getLight()->setAmbient(osg::Vec4(0.2, 0.2, 0.2, 1.0));
    lightOne->getLight()->setDiffuse(osg::Vec4(0.7, 0.4, 0.6, 1.0));
    lightOne->getLight()->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
    scene->addChild(lightOne.get());

    osg::ref_ptr<osg::LightSource> lightTwo = new osg::LightSource;
    lightTwo->getLight()->setLightNum(2); // GL_LIGHT2
    lightTwo->getLight()->setPosition(osg::Vec4(-10,10,15,1)); // 1 = ponctuelle
    lightTwo->getLight()->setAmbient(osg::Vec4(0.2, 0.2, 0.2, 1.0));
    lightTwo->getLight()->setDiffuse(osg::Vec4(0.7, 0.4, 0.6, 1.0));
    lightTwo->getLight()->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
    scene->addChild(lightTwo.get());

    osg::StateSet* state = scene->getOrCreateStateSet();
    state->setMode( GL_LIGHT0, osg::StateAttribute::OFF );
    state->setMode( GL_LIGHT1, osg::StateAttribute::OFF );
    state->setMode( GL_LIGHT2, osg::StateAttribute::ON );

    // Création d'une boîte centrée à l'origine, de dimensions 2x3x4:
    osg::Box* boite = new osg::Box(osg::Vec3(0, 0, 0), 2,3,4);
    osg::ShapeDrawable* boiteDrawable = new osg::ShapeDrawable(boite);
    osg::Geode* geodeBox = new osg::Geode();
    geodeBox->addDrawable(boiteDrawable);

    osg::Material* matBox = new osg::Material;
    matBox->setAmbient (osg::Material::FRONT_AND_BACK, osg::Vec4(0.5, 0.0, 0.0, 1.0));
    matBox->setDiffuse (osg::Material::FRONT_AND_BACK, osg::Vec4(0.9, 0.0, 0.0, 1.0));
    matBox->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.2, 0.2, 0.2, 1.0));
    matBox->setShininess(osg::Material::FRONT_AND_BACK, 64);
    boiteDrawable->getOrCreateStateSet()->setAttributeAndModes(matBox);

    // Création d'une sphère centrée à l'origine, de rayon 1:
    osg::Sphere* sphere = new osg::Sphere( osg::Vec3(0,0,0), 1.0);
    osg::ShapeDrawable* sphereDrawable =  new osg::ShapeDrawable(sphere);
    osg::Geode* geodeSphere = new osg::Geode();
    geodeSphere->addDrawable(sphereDrawable);

    osg::Material* matSphere = new osg::Material;
    matSphere->setAmbient (osg::Material::FRONT_AND_BACK, osg::Vec4(0.5, 0.5, 0.0, 1.0));
    matSphere->setDiffuse (osg::Material::FRONT_AND_BACK, osg::Vec4(0.9, 0.9, 0.0, 1.0));
    matSphere->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.2, 0.2, 0.2, 1.0));
    matSphere->setShininess(osg::Material::FRONT_AND_BACK, 64);
    sphereDrawable->getOrCreateStateSet()->setAttributeAndModes(matSphere);

    // Cone
    osg::Cone* cone = new osg::Cone(osg::Vec3(0,0,0), 1.0, 3.0);
    osg::ShapeDrawable* coneDrawable = new osg::ShapeDrawable(cone);
    osg::Geode* geodeCone = new osg::Geode();
    geodeCone->addDrawable(coneDrawable);

    osg::Material* matCone = new osg::Material;
    matCone->setAmbient (osg::Material::FRONT_AND_BACK, osg::Vec4(0.5, 0.0, 0.5, 1.0));
    matCone->setDiffuse (osg::Material::FRONT_AND_BACK, osg::Vec4(0.9, 0.0, 0.9, 1.0));
    matCone->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.2, 0.2, 0.2, 1.0));
    matCone->setShininess(osg::Material::FRONT_AND_BACK, 64);
    coneDrawable->getOrCreateStateSet()->setAttributeAndModes(matCone);


    // Application de changement matrice :
    // CUBE
    osg::PositionAttitudeTransform* transformCube = new osg::PositionAttitudeTransform();
    transformCube->setPosition(osg::Vec3(10, 0, 0));
    transformCube->setScale(osg::Vec3(1,1, 1));
    // Rotation de 20° par rapport à l'axe (0,0,1)
    transformCube->setAttitude(osg::Quat(osg::DegreesToRadians(20.0),osg::Vec3(0.0, 0.0, 1.0)));
    transformCube->addChild(geodeBox);

    // SPHERE
    osg::PositionAttitudeTransform* transformSphere = new osg::PositionAttitudeTransform();
    transformSphere->setPosition(osg::Vec3(0, 0, 10));
    transformSphere->setScale(osg::Vec3(1,1, 1));
    transformSphere->setAttitude(osg::Quat(osg::DegreesToRadians(0.0),osg::Vec3(0.0, 0.0, 1.0)));
    transformSphere->addChild(geodeSphere);

    // CONE
    osg::PositionAttitudeTransform* transformCone = new osg::PositionAttitudeTransform();
    transformCone->setPosition(osg::Vec3(0, 10, 0));
    transformCone->setScale(osg::Vec3(1,2, 2));
    transformCone->setAttitude(osg::Quat(osg::DegreesToRadians(0.0),osg::Vec3(0.0, 0.0, 1.0)));
    transformCone->addChild(geodeCone);



    /*osg::ref_ptr<osgGA::NodeTrackerManipulator> cameraCenter = new osgGA::NodeTrackerManipulator;
    cameraCenter->setTrackNode(geodeCone.get());
    cameraCenter->setTrackerMode(osgGA::NodeTrackerManipulator::NODE_CENTER);
    viewer.setCameraManipulator(cameraCenter->get());*/




    // Ajout des enfants au graph
    //scene->addChild(geodeBox);
    //scene->addChild(geodeSphere);
    //scene->addChild(geodeCone);

    transformCube->setName("Box");
    transformSphere->setName("Sphere");
    transformCone->setName("Cone");

    scene->addChild(transformCube);
    scene->addChild(transformSphere);
    scene->addChild(transformCone);

    viewer.setSceneData(scene);
    return viewer.run();
}
