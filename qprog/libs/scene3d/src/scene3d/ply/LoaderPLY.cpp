#include "LoaderPLY.h"
#include "../Tri.h"
#include <Message.h>
#include <math.h>
//#include <stdio.h>
//#include <strings.h>
#include "ply.h"

class LoaderPLYObject {
    public:
        Vec3 center;
        float radius;
    
        typedef struct {
            float x, y, z;
            float nx, ny, nz;
            void *other_props;       // other properties
            
            Vec3 vertex() const { return Vec3(x, y, z); }
            Vec3 normal() const { return Vec3(nx, ny, nz); }
            }
        Vertex;
    
        typedef struct {
            unsigned char nverts;    // number of vertex indices in list
            int *verts;              // vertex index list
            void *other_props;       // other properties
            //static PlyProperty props[];
            }
        Face;
    
        /*** the PLY object ***/
        int nverts, nfaces;
        Vertex **vlist;
        Face **flist;
        PlyOtherProp *vert_other, *face_other;
    
    public:
    
        LoaderPLYObject(QString fileName) : vlist(NULL), flist(NULL) {
            bool hasNx = false, hasNy = false, hasNz = false;
            bool flip_normals = true;      // flip vertex normals?
            
            //----------- Read in the original PLY object -----------
            // list of property information for a vertex
            PlyProperty vert_props[] = {
                {"x", Float32, Float32, offsetof(Vertex, x), 0, 0, 0, 0},
                {"y", Float32, Float32, offsetof(Vertex, y), 0, 0, 0, 0},
                {"z", Float32, Float32, offsetof(Vertex, z), 0, 0, 0, 0},
                {"nx", Float32, Float32, offsetof(Vertex, nx), 0, 0, 0, 0},
                {"ny", Float32, Float32, offsetof(Vertex, ny), 0, 0, 0, 0},
                {"nz", Float32, Float32, offsetof(Vertex, nz), 0, 0, 0, 0}
                };
            
            // list of property information for a face
            PlyProperty face_props[] = {
                {"vertex_indices", Int32, Int32, offsetof(Face, verts), 1, Uint8, Uint8, offsetof(Face, nverts)}
                };        
    
            FILE *file = fopen(fileName.toAscii().data(), "r");
            if (file == NULL) {
                Message::error(QString("probleme lors de l'ouverture du fichier '%1'").arg(fileName));
                return ; //@ voir : sceneLoaded = false
                }
            PlyFile *in_ply = read_ply(file);
    
            // examine each element type that is in the file (vertex, face)
            for (int i=0; i<in_ply->num_elem_types; i++) {
                // prepare to read the i'th list of elements
                int elem_count;
                char *elem_name = setup_element_read_ply(in_ply, i, &elem_count);
    
                if (equal_strings("vertex", elem_name)) {
                    // create a vertex list to hold all the vertices
                    vlist = new Vertex*[elem_count];
                    nverts = elem_count;
    
                    // set up for getting vertex elements (we want x,y,z)
                    setup_property_ply(in_ply, &vert_props[0]);
                    setup_property_ply(in_ply, &vert_props[1]);
                    setup_property_ply(in_ply, &vert_props[2]);
    
                    // we also want normal information if it is there (nx,ny,nz)
                    for (int j = 0; j < in_ply->elems[i]->nprops; j++) {
                        PlyProperty *prop;
                        prop = in_ply->elems[i]->props[j];
                        if (equal_strings("nx", prop->name)) {
                            setup_property_ply(in_ply, &vert_props[3]);
                            hasNx = true;
                            }
                            
                        if (equal_strings ("ny", prop->name)) {
                            setup_property_ply(in_ply, &vert_props[4]);
                            hasNy = true;
                            }
                        
                        if (equal_strings ("nz", prop->name)) {
                            setup_property_ply(in_ply, &vert_props[5]);
                            hasNz = true;
                            }
                        }
    
                    // also grab anything else that we don't need to know about
                    vert_other = get_other_properties_ply(in_ply, offsetof(Vertex, other_props));
    
                    // grab the vertex elements and store them in our list
    
                    for (int j = 0; j < elem_count; j++) {
                        vlist[j] = new Vertex();
                        get_element_ply(in_ply, (void *)vlist[j]);
                        }
                    }
                else if (equal_strings("face", elem_name)) {
                    // create a list to hold all the face elements
                    flist = new Face*[elem_count];
                    nfaces = elem_count;
    
                    // set up for getting face elements (all we need are vertex indices)
                    setup_property_ply(in_ply, &face_props[0]);
                    face_other = get_other_properties_ply(in_ply, offsetof(Face, other_props));
    
                    // grab all the face elements and place them in our list
                    for (int j = 0; j < elem_count; j++) {
                        flist[j] = new Face();
                        get_element_ply(in_ply, (void *) flist[j]);
                        }
                    }
                else
                    // all non-vertex and non-face elements are grabbed here
                    get_other_element_ply(in_ply);
                }
    
            close_ply(in_ply);
            free_ply(in_ply);
            }
    
        ~LoaderPLYObject() {
            if (vlist != NULL) {
                for (int i=0; i<nverts; i++) delete vlist[i];
                delete[] vlist;
                }
            if (flist != NULL) {
                for (int i=0; i<nfaces; i++) delete flist[i];
                delete[] flist;
                }
            //@@
            }
    
    
        };

TriSoup* LoaderPLY::loadRaw(QString fileName) const {
    LoaderPLYObject *object = new LoaderPLYObject(fileName);
    TriSoup *res = new TriSoup(object->nverts, object->nfaces, fileName);
    
    for (int i=0; i<object->nverts; i++)
        res->vertex[i] = object->vlist[i]->vertex();
    
    for (int i=0; i<object->nfaces; i++) {
        if (object->flist[i]->nverts == 3) {
            res->tri[i].index[0] = object->flist[i]->verts[0];
            res->tri[i].index[1] = object->flist[i]->verts[1];
            res->tri[i].index[2] = object->flist[i]->verts[2];
            }
        else {
            Message::info(QString("- attention : le modele contient des polygones à %1 cotés (pas supporté pour l'instant)").arg(object->flist[i]->nverts));
            delete object;
            delete res;
            return NULL;
            }
        }

    delete object;
    return res;
    }

