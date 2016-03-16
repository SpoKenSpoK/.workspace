#include <iostream>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>
//#include <osgGA/NodeTrackerManipulator>
#include <osg/LightSource>
//#include <osgGA/GUIEventHandler>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osg/Image>
#include <osg/Texture2D>
#include <osg/TexMat>
//#include <osg/Switch>
//#include <osgGA/GUIEventHandler>
//#include <osg/Fog>
//#include <osgParticle/PrecipitationEffect>
//#include <osgShadow/ShadowedScene>
//#include <osgShadow/ShadowMap>
//#include <osgShadow/SoftShadowMap>
//#include <osgGA/DriveManipulator>
//#include <osgText/Text>
//#include <osg/NodeCallback>
//#include <sstream>
#include <stdlib.h>

using namespace std;

// g++ -o prog cube.cpp -losg -losgDB -losgViewer



/*
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

*/

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

/*osg::ref_ptr<osg::Group> creation_troupeau(unsigned int nb_vaches, float taillex, float tailley){
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
}*/

osg::ref_ptr<osg::Node> creation_terrain(std::string nomTerrain, std::string nomTexture){
    osg::ref_ptr<osg::Image> heightMap = osgDB::readImageFile(nomTerrain);
    osg::ref_ptr<osg::HeightField> terrain = new osg::HeightField();
    terrain->allocate(heightMap->s(), heightMap->t());
    terrain->setOrigin(osg::Vec3(-heightMap->s() / 2, -heightMap->t() / 2, 0));
    terrain->setXInterval(10.0f);
    terrain->setYInterval(10.0f);
    for (unsigned int r = 0; r < terrain->getNumRows(); r++)
    for (unsigned int c = 0; c < terrain->getNumColumns(); c++)
    terrain->setHeight(c, r, ((*heightMap->data(c, r)) / 255.0f) * 300.0f);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();

    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setAmbient (osg::Material::FRONT_AND_BACK, osg::Vec4(0.5, 5.0, 5.0, 1.0));
    mat->setDiffuse (osg::Material::FRONT_AND_BACK, osg::Vec4(0.9, 0.9, 0.9, 1.0));
    mat->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0, 0.0, 0.0, 1.0));
    mat->setShininess(osg::Material::FRONT_AND_BACK, 64);
    osg::ref_ptr<osg::ShapeDrawable> fieldDrawable = new osg::ShapeDrawable(terrain);
    fieldDrawable->getOrCreateStateSet()->setAttributeAndModes(mat);

    osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D(osgDB::readImageFile(nomTexture));
    tex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR_MIPMAP_LINEAR);
    tex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
    tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex);

    osg::Matrixd matrix;
    matrix.makeScale(osg::Vec3(10, 10, 1.0));
    osg::ref_ptr<osg::TexMat> matTexture = new osg::TexMat;
    matTexture->setMatrix(matrix);
    geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, matTexture.get(), osg::StateAttribute::ON);

    geode->addDrawable(fieldDrawable);

    return geode;
}


bool intersection_terrain( float x, float y, osg::Node* terrain, osg::Vec3& inter, osg::Vec3& normal)
{
    osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(osg::Vec3(x,y,1000), osg::Vec3(x, y,-1000));
    osgUtil::IntersectionVisitor iv(intersector.get());
    terrain->accept(iv);
    if (intersector->containsIntersections()){
        osgUtil::LineSegmentIntersector::Intersections& intersections = intersector->getIntersections();
        // Le segment étant vertical, on prend la première (et à priori unique) intersection trouvée
        const osgUtil::LineSegmentIntersector::Intersection& intersection = *(intersections.begin());
        inter = intersection.getWorldIntersectPoint();
        normal = intersection.getWorldIntersectNormal();
        return true;
    }
    return false;
}


osg::Group* creation_foret( osg::ref_ptr<osg::Node> terrain, unsigned int nb_arbres){
    osg::ref_ptr<osg::Group> foret = new osg::Group;

    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setImage(osgDB::readImageFile("arbre.tga"));

    srand (time(NULL));
    for(unsigned int i=0; i<nb_arbres; ++i){
        float width = (rand() % 15)+5;
        float height = (rand() % 15)+5;

        float posx = (rand() % 500) - 250;
        float posy =(rand() % 500) -250;

        osg::Vec3 position(0.0,0.0,0.0);
        osg::Vec3 normale(0.0,0.0,0.0);
        intersection_terrain(posx, posy, terrain, position, normale);

        osg::ref_ptr<osg::Geometry> quad = osg::createTexturedQuadGeometry(
        osg::Vec3(-5.0, 0.0, 0.0),
        osg::Vec3(width, 0.0, 0.0),
        osg::Vec3(0.0, 0.0, height),
        0.0, 0.0, 1.0, 1.0);

        quad->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
        quad->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
        quad->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

        osg::ref_ptr<osg::Billboard> billboard = new osg::Billboard;
        billboard->setMode(osg::Billboard::POINT_ROT_EYE);
        billboard->addDrawable(quad);
        billboard->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

        osg::ref_ptr<osg::PositionAttitudeTransform> transform = new osg::PositionAttitudeTransform();
        transform->setPosition(position);
        transform->addChild(billboard);

        foret->addChild(transform);
        std::cerr<<"lol"<<std::endl;
    }

    std::cerr<<"DONE"<<std::endl;
    return foret;
}


int main(void)
{
    osgViewer::Viewer viewer;
    osg::ref_ptr<osg::Group> scene = new osg::Group;
    osg::DisplaySettings::instance()->setNumMultiSamples( 4 );
    viewer.addEventHandler(new osgViewer::StatsHandler);

    //camera = new osgGA::DriveManipulator();
    //viewer.setCameraManipulator(camera.get());

    viewer.setUpViewInWindow( 100, 50, 800, 600 );
    osgViewer::Viewer::Windows fenetres;
    viewer.getWindows(fenetres);
    fenetres[0]->setWindowName("Ein tank");
    viewer.getCamera()->setClearColor( osg::Vec4( 0,0.5,1,1 ) );

    osg::ref_ptr<osg::LightSource> lumiereOne = new osg::LightSource;
    lumiereOne->getLight()->setLightNum(0); // GL_LIGHT0
    lumiereOne->getLight()->setPosition(osg::Vec4(1,-1, 1, 0)); // 0 = directionnel
    lumiereOne->getLight()->setAmbient(osg::Vec4(0.5, 0.5, 0.5, 1.0));
    lumiereOne->getLight()->setDiffuse(osg::Vec4(0.9, 0.9, 0.9, 1.0));
    lumiereOne->getLight()->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
    scene->addChild(lumiereOne.get());

    //osg::ref_ptr<osgShadow::ShadowedScene> shadowScene = new osgShadow::ShadowedScene;
    //osg::ref_ptr<osgShadow::SoftShadowMap> sm = new osgShadow::SoftShadowMap;
    //osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
    //float douceur = 0.003;
    //sm->setSoftnessWidth(douceur);
    //shadowScene->setShadowTechnique(sm.get());
    //shadowScene->addChild(lumiereOne.get());
    //shadowScene->addChild(scene.get());

    //osg::StateSet* state = shadowScene->getOrCreateStateSet();
    //state->setMode( GL_LIGHT0, osg::StateAttribute::OFF );
    //state->setMode( GL_LIGHT1, osg::StateAttribute::ON );

    //osg::ref_ptr<osg::Switch> switchNode = new osg::Switch;
    //osg::ref_ptr<osg::Group> troupeau = creation_troupeau(150, 48, 48);
    //shadowScene->addChild(creation_sol(50, 50));
    //switchNode->addChild(troupeau);

    //osg::ref_ptr<GestionEvenements> gestionnaire = new GestionEvenements(scene, switchNode);
    //viewer.addEventHandler(gestionnaire.get());

    osg::ref_ptr<osg::Node> terrain = creation_terrain("terrain.tga","herbe.tga");
    osg::ref_ptr<osg::Group> foret = creation_foret(terrain, 10);
    std::cerr<<"Avant le addchild"<<std::endl;
    scene->addChild(foret);
    std::cerr<<"Après le addchild"<<std::endl;

    viewer.setSceneData(scene);
    return viewer.run();
}
