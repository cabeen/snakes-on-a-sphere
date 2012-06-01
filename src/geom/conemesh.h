/*!
   @file   Cone.h
   @author Ryan Cabeen (rpc@cs.brown.edu)
   @date   Fall 2010
*/

#ifndef __CONEMESH_H__    //header guards
#define __CONEMESH_H__

#include <common.h>
#include <mesh.h>
#include <QVector>
#include <matrix.h>

/**

 @class CONE
 @brief A shape implementation of a cone.

 @author  Ryan Cabeen (rpc)
 @date    Fall 2010

*/
class ConeMesh : public Mesh {
public:
    ConeMesh(int param1, int param2, float param3);
    virtual void setParams(int param1, int param2, float param3);

private:
    //! Build the shape
    virtual void build();
};

#endif // __CONEMESH_H__
