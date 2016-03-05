#include <iostream>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
//#include <osg/Material>
#include <osg/PositionAttitudeTransform>
//#include <osgGA/NodeTrackerManipulator>
//#include <osg/LightSource>
//#include <osgGA/GUIEventHandler>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <stdlib.h>
#include <osg/Switch>
#include <osgGA/GUIEventHandler>

using namespace std;

// g++ -o prog cube.cpp -losg -losgDB -losgViewer

class GestionEvenements : public osgGA::GUIEventHandler
{
    public:
    GestionEvenements(osg::ref_ptr<osg::Group> _scene, osg::ref_ptr<osg::Switch> _switch) {
        scene = _scene;
        switchNode = _switch;
    }
    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
    osg::ref_ptr<osg::Group> scene;
    osg::ref_ptr<osg::Switch> switchNode;
    bool on;
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
                if(on){
                    switchNode->setAllChildrenOff();
                    on = false;
                }

                else{
                    switchNode->setAllChildrenOn();
                    on = true;
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

osg::ref_ptr<osg::Group> creation_troupeau(unsigned int nb_vaches, float taillex, float tailley){
    osg::ref_ptr<osg::Group> troupeau = new osg::Group;
    osg::ref_ptr<osg::LOD> lod = new osg::LOD;
    lod->setRangeMode( osg::LOD::DISTANCE_FROM_EYE_POINT );

    osg::ref_ptr<osg::Node> vacheH = osgDB::readNodeFile("cow_high.3ds");
    lod->addChild( vacheH, 0, 2500);
    osg::ref_ptr<osg::Node> vacheM = osgDB::readNodeFile("cow_mid.3ds");
    lod->addChild( vacheM, 2500, 8500);
    osg::ref_ptr<osg::Node> vacheL = osgDB::readNodeFile("cow_low.3ds");
    lod->addChild( vacheL, 8500, 100000);

    srand (time(NULL));
    for(unsigned int i=0; i<nb_vaches; ++i){
        float x = rand() % (int)taillex+1;
        float y = rand() % (int)tailley+1;

        osg::ref_ptr<osg::PositionAttitudeTransform> posvache = new osg::PositionAttitudeTransform();
        posvache->setPosition(osg::Vec3(x, y, 570));
        posvache->setScale(osg::Vec3(1,1,1));

        float degree = rand() % 361;
        // Rotation de 20° par rapport à l'axe (0,0,1)
        posvache->setAttitude(osg::Quat(osg::DegreesToRadians(degree),osg::Vec3(0.0, 0.0, 1.0)));
        posvache->addChild(lod);
        troupeau->addChild(posvache);
    }

    return troupeau;
}

osg::ref_ptr<osg::Node> creation_sol(float taillex, float tailley){
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setImage(osgDB::readImageFile("herbe.jpg"));
    osg::ref_ptr<osg::Geometry> quad = osg::createTexturedQuadGeometry(
    osg::Vec3(-1000,-1000, 0.0), // Coin de départ
    osg::Vec3(taillex, 0.0, 0.0), // largeur
    osg::Vec3(0.0, tailley, 0.0), // hauteur
    0.0, 0.0, 1.0, 1.0); // Coordonnées de texture gauche/bas/droit/haut
    quad->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
    quad->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(quad);

    return geode;
}

int main(void)
{
    osgViewer::Viewer viewer;
    osg::ref_ptr<osg::Group> scene = new osg::Group;
    osg::DisplaySettings::instance()->setNumMultiSamples( 4 );
    viewer.addEventHandler(new osgViewer::StatsHandler);

    osg::ref_ptr<osg::Switch> switchNode = new osg::Switch;

    viewer.setUpViewInWindow( 100, 50, 800, 600 );
    osgViewer::Viewer::Windows fenetres;
    viewer.getWindows(fenetres);
    fenetres[0]->setWindowName("Vachette");
    viewer.getCamera()->setClearColor( osg::Vec4( 0,0.5,1,1 ) );

    osg::ref_ptr<osg::Group> troupeau = creation_troupeau(20, 10000, 10000);
    scene->addChild(creation_sol(12000, 12000));
    switchNode->addChild(troupeau);

    osg::ref_ptr<GestionEvenements> gestionnaire = new GestionEvenements(scene, switchNode);
    viewer.addEventHandler(gestionnaire.get());

    scene->addChild(switchNode.get());

    viewer.setSceneData(scene);
    return viewer.run();
}
