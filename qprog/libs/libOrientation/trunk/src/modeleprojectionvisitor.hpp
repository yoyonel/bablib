#ifndef __MODELEPROJECTIONVISITOR_HPP__
#define __MODELEPROJECTIONVISITOR_HPP__

namespace OrientationMATIS
{

class ModeleProjectionSpherique;
class ModeleProjectionAxono;
class ModeleProjectionCarto;
class ModeleProjectionConique;
class ModeleProjectionGrille;
class ModeleProjectionRTO;

class ModeleProjectionVisitor
{
public :
	virtual bool Visit(const ModeleProjectionSpherique *);
	virtual bool Visit(const ModeleProjectionAxono *);
	virtual bool Visit(const ModeleProjectionCarto *);
	virtual bool Visit(const ModeleProjectionConique *);
	virtual bool Visit(const ModeleProjectionGrille *);
	virtual bool Visit(const ModeleProjectionRTO *);
	virtual ~ModeleProjectionVisitor() {}
};

}
#endif // #ifndef __MODELEPROJECTIONVISITOR_HPP__

