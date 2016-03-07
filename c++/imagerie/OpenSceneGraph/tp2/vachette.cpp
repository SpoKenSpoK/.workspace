#include <iostream>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
//#include <osg/Material>
#include <osg/PositionAttitudeTransform>
//#include <osgGA/NodeTrackerManipulator>
#include <osg/LightSource>
//#include <osgGA/GUIEventHandler>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <stdlib.h>
#include <osg/Switch>
#include <osgGA/GUIEventHandler>
#include <osg/Fog>
#include <osgParticle/PrecipitationEffect>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgGA/DriveManipulator>
#include <osgText/Text>
#include <osg/NodeCallback>
#include <sstream>

using namespace std;

// g++ -o prog cube.cpp -losg -losgDB -losgViewer

osg::ref_ptr<osgGA::DriveManipulator> camera;
osg::ref_ptr<osgText::Text> text = new osgText::Text;


class callbackSpeed : public osg::NodeCallback
{
    public:
        virtual void operator() (osg::Node* n, osg::NodeVisitor* nv)
        {
            double speed = camera->getVelocity();
            std::ostringstream oss;
            oss << speed;
            std::string os = oss.str();
            text->setText(os);
        }
};

osg::Node* creationHUD()
{
    // On crée une caméra qui correspond à un écran de 1280x1024
    osg::Camera* camera = new osg::Camera;
    camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1280, 0, 1024));
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
    // Le sous-graphe de la caméra sera affiché après celui de la caméra principale,
    // donc par dessus le reste de la scène.
    camera->setRenderOrder(osg::Camera::POST_RENDER);
    // Les éléments graphiques du HUD (ici un simple texte) constitueront un sous-graphe
    // de la caméra que l'on vient de créer
    text->setPosition(osg::Vec3(50.0f, 50.0f, 0.0f));
    //text->setText(speed_txt);
    text->setCharacterSize(20);
    text->setFont("arial.ttf");
    osg::Geode* geode = new osg::Geode();
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    geode->addDrawable(text);
    camera->addChild(geode);
    return camera;
}

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
    lod->addChild( vacheH, 0, 15);
    osg::ref_ptr<osg::Node> vacheM = osgDB::readNodeFile("cow_mid.3ds");
    lod->addChild( vacheM, 15, 40);
    osg::ref_ptr<osg::Node> vacheL = osgDB::readNodeFile("cow_low.3ds");
    lod->addChild( vacheL, 40, 1000);

    srand (time(NULL));
    for(unsigned int i=0; i<nb_vaches; ++i){
        float x = rand() % (int)taillex+1;
        float y = rand() % (int)tailley+1;

        osg::ref_ptr<osg::PositionAttitudeTransform> posvache = new osg::PositionAttitudeTransform();
        posvache->setPosition(osg::Vec3(x, y, 0));
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
    osg::Vec3(0.0, 0.0, 0.0), // Coin de départ
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
    camera = new osgGA::DriveManipulator();
    viewer.setCameraManipulator(camera.get());

    viewer.setUpViewInWindow( 100, 50, 800, 600 );
    osgViewer::Viewer::Windows fenetres;
    viewer.getWindows(fenetres);
    fenetres[0]->setWindowName("Vachette");
    viewer.getCamera()->setClearColor( osg::Vec4( 0,0.5,1,1 ) );

    osg::ref_ptr<osg::LightSource> lumiereOne = new osg::LightSource;
    lumiereOne->getLight()->setLightNum(1); // GL_LIGHT1
    lumiereOne->getLight()->setPosition(osg::Vec4(1,-1, 1, 0)); // 0 = directionnel
    lumiereOne->getLight()->setAmbient(osg::Vec4(0.2, 0.2, 0.2, 1.0));
    lumiereOne->getLight()->setDiffuse(osg::Vec4(0.4, 0.4, 0.4, 1.0));
    lumiereOne->getLight()->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
    //scene->addChild(lumiereOne.get());

    osg::ref_ptr<osgShadow::ShadowedScene> shadowScene = new osgShadow::ShadowedScene;
    osg::ref_ptr<osgShadow::SoftShadowMap> sm = new osgShadow::SoftShadowMap;
    //osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
    float douceur = 0.003;
    //sm->setSoftnessWidth(douceur);
    //shadowScene->setShadowTechnique(sm.get());
    shadowScene->addChild(lumiereOne.get());
    shadowScene->addChild(scene.get());

    osg::StateSet* state = shadowScene->getOrCreateStateSet();
    state->setMode( GL_LIGHT0, osg::StateAttribute::OFF );
    state->setMode( GL_LIGHT1, osg::StateAttribute::ON );

    osg::ref_ptr<osg::Switch> switchNode = new osg::Switch;
    osg::ref_ptr<osg::Group> troupeau = creation_troupeau(150, 48, 48);
    shadowScene->addChild(creation_sol(50, 50));
    switchNode->addChild(troupeau);

    osg::ref_ptr<GestionEvenements> gestionnaire = new GestionEvenements(scene, switchNode);
    viewer.addEventHandler(gestionnaire.get());
    shadowScene->addChild(switchNode.get());

    /*osg::Fog* pFog = new osg::Fog();

    pFog->setMode(osg::Fog::LINEAR);
    pFog->setColor(osg::Vec4(0.5, 0.5, 0.5, 1));
    pFog->setStart(1);
    pFog->setEnd(250);
    scene->getOrCreateStateSet()->setAttribute(pFog, osg::StateAttribute::OFF);
    scene->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::OFF); */

    osg::ref_ptr<osgParticle::PrecipitationEffect> precipNode = new osgParticle::PrecipitationEffect;

    float xdir = 12.0f;
    float ydir = 0.0f;
    float zdir = 0.0f;
    precipNode->setWind(osg::Vec3(xdir,ydir,zdir));
    precipNode->setParticleSpeed(0.4);
    precipNode->rain(0.4); // ou « snow » pour de la neige
    scene->addChild(precipNode.get());
    switchNode->setUpdateCallback(new callbackSpeed);
    scene->addChild(creationHUD());

    viewer.setSceneData(shadowScene);
    //viewer.setSceneData(scene);
    return viewer.run();
}
