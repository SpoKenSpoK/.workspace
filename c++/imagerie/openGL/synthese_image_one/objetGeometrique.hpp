#ifndef OBJETGEOMETRIQUE_HPP
#define OBJETGEOMETRIQUE_HPP

class objetGeometrique {
private:
	unsigned int nbPts;

protected:
    float* vertices;

public:
	inline objetGeometrique(unsigned int _nbPts): nbPts(_nbPts) {};
    inline virtual ~objetGeometrique() { if(vertices != 0) delete [] vertices; };
	virtual void calculPosition_sommets() =0;
    inline float* returnVertices() const { return vertices; }
    inline const unsigned int & getNbP() const { return nbPts; }
    inline void setNbP(const int& _nbPts) { nbPts = _nbPts; }
};

#endif
