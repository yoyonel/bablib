#include "ScenePLY.h"
#include "AABox.h"
#include <Message.h>
#include <glide/glarraybuffers.h>
#include <math.h>

//#include <stdio.h>
//#include <strings.h>
#include "ply.h"

using namespace glide;

class ScenePLY::Object {
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

    Object(QString fileName) : vlist(NULL), flist(NULL) {
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

        //////////////////////////////////////////////////////
        // post-processes :
        
        //@@@ faire verif coherence donnees lues (index compis entre 0 et nverts-1)
        /*
        MSG_DEBUG_VAR(nverts);
        MSG_DEBUG_VAR(nfaces);
        for (int i=0; i<nfaces; i++) {
            for (int j=0; j<flist[i]->nverts; j++) {
                int k = flist[i]->verts[j];
                if (k <0 || k >= nverts) {
                    //MSG_DEBUG_VAR(k);
                    flist[i]->verts[j] = abs(k) % nverts;
                    if (flist[i]->verts[j] < 0) flist[i]->verts[j] = 0;
                    }
                }
            }
        */
        
        // calcul bbox :
        AABox box;
        for (int i = 0; i < nverts; i++)
            box.add(vlist[i]->vertex());
        center = box.center();
        radius = box.radius();
        
        // calcul normales :
        if (!(hasNx && hasNy && hasNz))
            computeNormals(flip_normals);
        }

    ~Object() {
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


    void computeNormals(bool flip_sign) {
        int i, j;
        Face *face;
        Vertex *vert;
        int *verts;
        float x, y, z;
        float x0, y0, z0;
        float x1, y1, z1;
        float len;
        float recip;

        /* zero out all normal information at vertices */

        for (i = 0; i < nverts; i++) {
            vlist[i]->nx = 0;
            vlist[i]->ny = 0;
            vlist[i]->nz = 0;
            }

        /* find normal of each face and add it to each vertex adjacent to the face */

        for (i = 0; i < nfaces; i++) {

            face = flist[i];
            verts = face->verts;

            /* determine vectors parallel to two edges of face */

            x0 = vlist[verts[face->nverts - 1]]->x - vlist[verts[0]]->x;
            y0 = vlist[verts[face->nverts - 1]]->y - vlist[verts[0]]->y;
            z0 = vlist[verts[face->nverts - 1]]->z - vlist[verts[0]]->z;

            x1 = vlist[verts[1]]->x - vlist[verts[0]]->x;
            y1 = vlist[verts[1]]->y - vlist[verts[0]]->y;
            z1 = vlist[verts[1]]->z - vlist[verts[0]]->z;

            /* find cross-product between these vectors */
            x = y0 * z1 - z0 * y1;
            y = z0 * x1 - x0 * z1;
            z = x0 * y1 - y0 * x1;

            /* normalize this vector */
            len = x * x + y * y + z * z;
            if (len == 0) {
                x = y = z = 0;
                } else {
                recip = 1 / sqrtf(len);
                x *= recip;
                y *= recip;
                z *= recip;
                }

            /* add this normal to each vertex that is adjacent to face */
            for (j = 0; j < face->nverts; j++) {
                vlist[verts[j]]->nx += x;
                vlist[verts[j]]->ny += y;
                vlist[verts[j]]->nz += z;
                }
            }

        /* normalize all the normals at the vertices */

        for (i = 0; i < nverts; i++) {
            vert = vlist[i];
            len = vert->nx * vert->nx + vert->ny * vert->ny + vert->nz * vert->nz;
            if (len == 0) {
                vert->nx = 0;
                vert->ny = 0;
                vert->nz = 0;
                } else {
                if (flip_sign)
                    recip = -1 / sqrtf(len);
                else
                    recip = 1 / sqrtf(len);
                vert->nx *= recip;
                vert->ny *= recip;
                vert->nz *= recip;
                }
            }
        }

    };

ScenePLY::ScenePLY(QString fileName) : Scene3D(fileName), geometry(NULL) {
    object = new Object(fileName);
    }

ScenePLY::~ScenePLY() {
    delete object;
    delete geometry;
    }

void ScenePLY::init() {
    GLArrayBufferCPU<AL_2(Vertex3f,Normal3f)> vertexArray;

    for (int i=0; i<object->nverts; i++) {
        vertexArray.glNormal3fv(object->vlist[i]->normal());
        vertexArray.glVertex3fv(object->vlist[i]->vertex());
        }
    
    GLElementArrayBufferCPU indexes;
    for (int i=0; i<object->nfaces; i++) {
        if (object->flist[i]->nverts == 3) {
            indexes += object->flist[i]->verts[0];
            indexes += object->flist[i]->verts[1];
            indexes += object->flist[i]->verts[2];
            }
        else
            Message::info(QString("- attention : le modele contient des polygones ÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ  %1 cotÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ©s (pas supportÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ© pour l'instant)").arg(object->flist[i]->nverts));
        }

    geometry = new GLAsset::Mesh(GL_TRIANGLES, vertexArray.mappedToGPU(), indexes.mappedToGPU());
    geometry->mapToGPU();
    initialized = true;
    }
        
void ScenePLY::render() const {
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glColor3f(0.8f, 0.8f, 0.8f);
    
    if (geometry != NULL) geometry->render();
    }

Vec3 ScenePLY::getCenter() const  { return object->center; }
float ScenePLY::getRadius() const { return object->radius; }

