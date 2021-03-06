#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgDB/ReadFile>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/GUIEventHandler>
#include <osgViewer/ViewerEventHandlers>
#include <osg/TexMat>
#include <osg/LightSource>
#include <osgUtil/LineSegmentIntersector>
#include <ctime>
#include <cstdlib>
#include <osgSim/DOFTransform>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osgParticle/SmokeEffect>

osgViewer::Viewer viewer;
osg::ref_ptr<osg::Node> terrain;

float angle = 0.0f;

class ChercheNoeud : public osg::NodeVisitor
{
public:
	inline ChercheNoeud ( const std::string& name )
	: osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ), _name( name ) {}
	inline virtual void apply( osg::Node& node ) {
		if (node.getName() == _name)
			_node = &node;
		traverse( node );
	}
	osg::Node* getNode() { return _node.get(); }

protected:
	std::string _name;
	osg::ref_ptr<osg::Node> _node;
};


bool intersection_terrain( float x, float y, osg::Node* terrain, osg::Vec3& inter, osg::Vec3& normal)
{
	osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
	new osgUtil::LineSegmentIntersector(osg::Vec3(x,y,1000), osg::Vec3(x, y,-1000));
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

class AnimationTank : public osg::AnimationPathCallback
{
public :
    AnimationTank(osg::AnimationPath* p, osg::Node* _terrain)
		: osg::AnimationPathCallback(p), terrain(_terrain) {}
    virtual void operator()(osg::Node*  node, osg::NodeVisitor*  nv)
    {
        osg::AnimationPathCallback::operator()(node, nv);
        osg::PositionAttitudeTransform* transform =
		dynamic_cast<osg::PositionAttitudeTransform*>(node);
         if (transform)
         {
            osg::Vec3 position = transform->getPosition();
			osg::Vec3 normale;
			intersection_terrain(position.x(), position.y(), terrain, position, normale);
			osg::Quat rotate;
			rotate.makeRotate(osg::Vec3f(0, 0, 1), normale);
			transform->setAttitude(rotate);
			transform->setPosition(position);
        }
    }
   osg::Node* terrain;
};





class Deplacement : public osg::NodeCallback
{
public:
    virtual void operator() (osg::Node* n, osg::NodeVisitor* nv)
    {
		osg::PositionAttitudeTransform* pos_tank = (osg::PositionAttitudeTransform*)n;
		osg::Vec3 pos, normal;
		intersection_terrain(pos_tank->getPosition().x(), pos_tank->getPosition().y(), terrain, pos, normal);
		pos_tank->setPosition(pos);
		osg::Quat rotation;
		rotation.makeRotate(osg::Vec3f(0, 0, 1), normal);
		pos_tank->setAttitude(rotation);

		ChercheNoeud rechercheTourelle("turret");
		pos_tank->accept(rechercheTourelle);
		osg::Node* noeudTourelle = rechercheTourelle.getNode();
		if (noeudTourelle != NULL){
			osgSim::DOFTransform* tourelleDOF = dynamic_cast<osgSim::DOFTransform*>(noeudTourelle);
			tourelleDOF->setCurrentHPR(osg::Vec3(osg::DegreesToRadians(angle),0.0,0.0));
		}
    }
};

class GestionEvenements : public osgGA::GUIEventHandler
{
 public:
 virtual bool handle( const osgGA::GUIEventAdapter& ea,
 osgGA::GUIActionAdapter& aa);
};


osg::Node* creation_terrain(){
	osg::Image* heightMap = osgDB::readImageFile("terrain.tga");

	osg::HeightField* terrain = new osg::HeightField();
	terrain->allocate(heightMap->s(), heightMap->t());
	terrain->setOrigin(osg::Vec3(-heightMap->s() / 2, -heightMap->t() / 2, 0));
	terrain->setXInterval(10.0f);
	terrain->setYInterval(10.0f);

	for (unsigned int r = 0; r < terrain->getNumRows(); r++)
		for (unsigned int c = 0; c < terrain->getNumColumns(); c++)
			terrain->setHeight(c, r, ((*heightMap->data(c, r)) / 255.0f) * 300.0f);

	osg::Geode* geode = new osg::Geode();
	geode->addDrawable(new osg::ShapeDrawable(terrain));

	osg::Material* mat = new osg::Material;

	mat->setAmbient (osg::Material::FRONT_AND_BACK, osg::Vec4(0.5, 0.5, 0.5, 1.0));
	mat->setDiffuse (osg::Material::FRONT_AND_BACK, osg::Vec4(0.9, 0.9, 0.9, 1.0));
	mat->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0, 0.0, 0.0, 1.0));
	mat->setShininess(osg::Material::FRONT_AND_BACK, 64);
	geode->getOrCreateStateSet()->setAttributeAndModes(mat);

	osg::Texture2D* tex = new osg::Texture2D(osgDB::readImageFile("herbe.tga"));
	tex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR_MIPMAP_LINEAR);
	tex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
	tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex);

	osg::Matrixd matrix;
	matrix.makeScale(osg::Vec3(10, 10, 1.0));
	osg::ref_ptr<osg::TexMat> matTexture = new osg::TexMat;
	matTexture->setMatrix(matrix);
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,
	matTexture.get(), osg::StateAttribute::ON);

	return geode;

}

osg::ref_ptr<osg::Group> creation_foret(osg::Node* terrain, int nb_arbres){
	osg::ref_ptr<osg::Group> foret = new osg::Group;
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D(osgDB::readImageFile("arbre.tga"));
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);

	srand (static_cast <unsigned> (time(0)));
	float arbre_posx, arbre_posy, arbre_taille;
	for(int i=0; i<nb_arbres; ++i){

		arbre_posx = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1000));
		arbre_posy = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1000));
		arbre_taille = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/15)) + 5.0;
		osg::ref_ptr<osg::Geometry> quad = osg::createTexturedQuadGeometry(
			osg::Vec3(-arbre_taille/2, 0, 0), // coin de départ
			osg::Vec3(arbre_taille, 0.0, 0.0), // largeur
			osg::Vec3(0.0, 0.0, arbre_taille), // hauteur
			0.0, 0.0, 1.0, 1.0); // coordonnées de texture
		quad->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
		quad->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
		// On ajoute le quad dans le TRANSPARENT BIN, une collection d'objets transparents,
		// pour qu'ils soient triés selon la profondeur et affichés du plus éloigné au plus proche
		quad->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		// On crée le billboard et on lui attache le quad à orienter face à la caméra
		osg::ref_ptr<osg::Billboard> billboard = new osg::Billboard;
		billboard->setMode(osg::Billboard::AXIAL_ROT);
		billboard->setAxis(osg::Vec3(0,0,1));
		billboard->addDrawable(quad);
		// Optionnel : on désactive l'éclairage sur le billboard
		billboard->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		osg::PositionAttitudeTransform* pos_arbre = new osg::PositionAttitudeTransform;
		osg::Vec3 pos, normal;
		intersection_terrain(arbre_posx, arbre_posy, terrain, pos, normal);
		pos_arbre->setPosition(pos);
		pos_arbre->addChild(billboard);
		foret->addChild(pos_arbre);
	}

	return foret;
}

osg::PositionAttitudeTransform* creation_CHARRR(float posx, float posy, osg::Node* terrain){
	osg::Node* LECHARRR = osgDB::readNodeFile("t72-tank_des.flt");
	osg::PositionAttitudeTransform* pos_tank = new osg::PositionAttitudeTransform;
	osg::Vec3 pos, normal;
	intersection_terrain(posx, posy, terrain, pos, normal);
	pos_tank->setPosition(pos);
	osg::Quat rotation;
	rotation.makeRotate(osg::Vec3f(0, 0, 1), normal);
	pos_tank->setAttitude(rotation);
	pos_tank->addChild(LECHARRR);
	pos_tank->setScale(osg::Vec3f(10.0,10.0,10.0));
	pos_tank->setUpdateCallback(new Deplacement);

	osg::ref_ptr<osgParticle::SmokeEffect> effectNode = new osgParticle::SmokeEffect; // Création fumée
	effectNode->setTextureFileName("smoke.png");
	effectNode->setIntensity(2);
	effectNode->setScale(2);
	effectNode->setPosition(osg::Vec3(pos_tank->getPosition().x(),pos_tank->getPosition().y()-50,pos_tank->getPosition().z()));
	pos_tank->addChild(effectNode.get());



	return pos_tank;
}

bool GestionEvenements::handle( const osgGA::GUIEventAdapter& ea,
 osgGA::GUIActionAdapter& aa)
{
	switch(ea.getEventType()){
		case osgGA::GUIEventAdapter::KEYDOWN :

			switch(ea.getKey()){

				case 'a':
					angle+=5;
					break;
				case 'z':
					angle-=5;
					break;
				case 'e':
					break;
			}
			break;

		case osgGA::GUIEventAdapter::PUSH :{
			int x = ea.getX();
			int y = ea.getY();
			if( ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
				std::cout << "bouton gauche" << std::endl;
			if (ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON)
				std::cout << "bouton milieu" << std::endl;
			if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
				std::cout << "bouton droit" << std::endl;
			break;
 }
		case osgGA::GUIEventAdapter::DOUBLECLICK :
			break;
			}
 return false; // pour que l'événement soit traité par d'autres gestionnaires
}

int main(void){
	osg::DisplaySettings::instance()->setNumMultiSamples( 4 );
	viewer.setUpViewInWindow( 100, 50, 800, 600 );
	viewer.getCamera()->setClearColor( osg::Vec4( 0.5,0.5,0.5,1) );
	viewer.addEventHandler(new osgViewer::StatsHandler);

	osg::Group* scene = new osg::Group;

	osg::ref_ptr<osg::LightSource> lumiere = new osg::LightSource;
	lumiere->getLight()->setLightNum(0); // GL_LIGHT1
	lumiere->getLight()->setPosition(osg::Vec4(1, -1, 2, 0)); // 0 = directionnel
	lumiere->getLight()->setAmbient(osg::Vec4(0.5, 0.5, 0.5, 1.0));
	lumiere->getLight()->setDiffuse(osg::Vec4(0.9, 0.9, 0.9, 1.0));
	lumiere->getLight()->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	scene->addChild(lumiere.get());

	terrain = creation_terrain();
	scene->addChild(terrain.get());
	scene->addChild(creation_foret(terrain, 2500));

	osg::ref_ptr<osgGA::NodeTrackerManipulator> manip = new osgGA::NodeTrackerManipulator;
	osg::ref_ptr<osg::Node> tank = creation_CHARRR(0,0,terrain);

	manip->setTrackNode(tank.get());
	manip->setTrackerMode(osgGA::NodeTrackerManipulator::NODE_CENTER);
	//viewer.setCameraManipulator(manip.get());

	//osg::ref_ptr<osg::PositionAttitudeTransform> mt = new osg::PositionAttitudeTransform; // PAT sur le Tank
	//mt->addChild( tank ); // Ajout du node tank au PAT
	osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath; // Animation PAT
	// Définition du mode de boucle (autres possibilités : LOOP, NO_LOOPING)
	path->setLoopMode( osg::AnimationPath::LOOP );
	// Création de points de contrôle
	osg::AnimationPath::ControlPoint  p0(osg::Vec3(0,0,0));
	osg::AnimationPath::ControlPoint  p1(osg::Vec3( 0,1200,0));
	osg::AnimationPath::ControlPoint  p2(osg::Vec3( 1200,1200,0));
	osg::AnimationPath::ControlPoint  p3(osg::Vec3( 1200,0,0));
	osg::AnimationPath::ControlPoint  p4(osg::Vec3( 0,0,0));
	path->insert( 0.0f, p0 );
	path->insert( 4.0f, p1 );
	path->insert( 8.0f, p2 );
	path->insert( 12.0f, p3 );
	path->insert( 16.0f, p4 );
	osg::ref_ptr<osg::AnimationPathCallback> apc = new AnimationTank(path, terrain);
	tank->setUpdateCallback( apc.get() );

	scene->addChild(tank);


	viewer.setSceneData(scene);

	osg::ref_ptr<GestionEvenements> gestionnaire = new GestionEvenements();
	viewer.addEventHandler(gestionnaire.get());

	return viewer.run();
}
