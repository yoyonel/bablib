#include <OpenGL.h>
#include "Scene3DS.h"
#include <Image2D.h>
#include <ColorTypes.h>
#include <Texture.h>
#include <Message.h>
#include <lib3ds/camera.h>
#include <lib3ds/mesh.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>
#include <lib3ds/vector.h>
#include <lib3ds/light.h>
#include <lib3ds/file.h>
#include <lib3ds/node.h>

#ifndef fmaxf
#define fmaxf(_a, _b) (float)(_a) > (float)(_b) ? (float)(_a) : (float)(_b)
#endif

//@ a faire :
// - gestion des textures (cf 3dsplay.c)
// - mode diffuse seulement
// - problemes transfos ?
// - problemes calcul normales (groupes de lissage, sens) ?
// - gestion lights / cameras
// - infos modele (nb triangles, nb vertices)
// - utiliser VBOs / DispListGL


Scene3DS::Scene3DS(QString fileName) : Scene3D(fileName), displist(0), center(0,0,0), radius(1) {
    file = lib3ds_file_load(fileName.toAscii().data());
    if (file != NULL) {
        lib3ds_file_eval(file, 0);
        //calcNombreTriangles();

        // calcul de la bounding sphere :
        Vec3 bmin, bmax;
        lib3ds_file_bounding_box_of_objects(file, true, false, false, bmin, bmax);
        center = (bmin + bmax) / 2;
        radius = dist(center, bmax);
        }
    else
        Message::error(QString("problème pour charger le fichier '%1'").arg(fileName));
    }

Scene3DS::~Scene3DS() {
    //@ + a voir
    lib3ds_file_free(file);
    }

void Scene3DS::init() {
    if (file == NULL) return;

    if (displist <= 0) {
        Lib3dsMaterial *mat = file->materials;
        while (mat != NULL) {
            MSG_DEBUG_VAR(mat->name);
            
            if (mat->texture1_map.name[0] != 0) {
                Lib3dsTextureMap &map = mat->texture1_map;
                QString texFileName = QString(map.name).toLower();
                if (texFileName.endsWith(".tif", Qt::CaseInsensitive))
                    texFileName = texFileName.remove(".tif", Qt::CaseInsensitive) + ".png";
                
                Image2D<Float4> image(texFileName);
                if (!image.loaded()) {
                    Message::error(QString("probleme pour charger la texture '%1'").arg(texFileName));
                    }
                else {
                    Texture *tex = new Texture(&image, GL_LINEAR, GL_CLAMP_TO_EDGE);
                    tex->load();
                    image.destroy();
                    
                    map.user.p = static_cast<void*>(tex);
                    }
                }
            //if (mat->texture1_map.name[0] != 0) MSG_DEBUG_VAR(mat->texture1_map.name);
            /*
            if (mat->texture1_mask.name[0] != 0) MSG_DEBUG_VAR(mat->texture1_mask.name);
            if (mat->texture2_map.name[0] != 0) MSG_DEBUG_VAR(mat->texture2_map.name);
            if (mat->texture2_mask.name[0] != 0) MSG_DEBUG_VAR(mat->texture2_mask.name);
            */
            
            mat = mat->next;
            }
        
        displist = glGenLists(1);
        glNewList(displist, GL_COMPILE);
            glShadeModel(GL_SMOOTH);
            glEnable(GL_LIGHTING);
            glDisable(GL_LIGHTING);
            glDisable(GL_COLOR_MATERIAL);
            //glEnable(GL_COLOR_MATERIAL);
            
            GLfloat modelAmbient[] = { 0.0, 0.0, 0.0, 1.0 };
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, modelAmbient);
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
            
            GLfloat amb[] = { 0.0, 0.0, 0.0, 1.0 };
            //GLfloat dif[] = { 1.0, 1.0, 1.0, 1.0 };
            GLfloat dif[] = { 0.8, 0.8, 0.8, 1.0 };
            //GLfloat spe[] = { 1.0, 1.0, 1.0, 1.0 };
            GLfloat spe[] = { 0.5, 0.5, 0.5, 1.0 };
                
            glLightfv(GL_LIGHT0, GL_AMBIENT,  amb);
            glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif);
            glLightfv(GL_LIGHT0, GL_SPECULAR, spe);
                
            glDisable(GL_CULL_FACE);
            //glEnable(GL_CULL_FACE);
            //glCullFace(GL_BACK);
            
            /*
            static const float noir[4] = {0, 0, 0, 1};
            switch(modeDessin) {
                case DESSIN_AVEC_ECLAIRAGE:
                    glLightfv(GL_LIGHT0, GL_SPECULAR, noir);
                    //glLightfv(GL_LIGHT0, GL_AMBIENT,  noir);
                    glEnable(GL_LIGHTING);
                    //glDisable(GL_COLOR_MATERIAL);
                    glEnable(GL_COLOR_MATERIAL);
                    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
                    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, noir);
                    break;
                case DESSIN_SANS_ECLAIRAGE:
                    glDisable(GL_LIGHTING);
                    glEnable(GL_COLOR_MATERIAL);
                    break;
                }
            */
            
            /*
            // Lights
            GLfloat amb[] = {0.0, 0.0, 0.0, 1.0};
            GLfloat dif[] = {1.0, 1.0, 1.0, 1.0};
            GLfloat spe[] = {1.0, 1.0, 1.0, 1.0};
            GLfloat pos[] = {0.0, 0.0, 0.0, 1.0};
            int li=GL_LIGHT0;
            for (Lib3dsLight* l=file->lights; l; l=l->next) {
                glEnable(li);
                
                glLightfv(li, GL_AMBIENT,  amb);
                glLightfv(li, GL_DIFFUSE,  dif);
                glLightfv(li, GL_SPECULAR, spe);
                
                pos[0] = l->position[0];
                pos[1] = l->position[1];
                pos[2] = l->position[2];
                glLightfv(li, GL_POSITION, pos);
                
                if (!l->spot_light) continue;
                
                pos[0] = l->spot[0] - l->position[0];
                pos[1] = l->spot[1] - l->position[1];
                pos[2] = l->spot[2] - l->position[2];      
                glLightfv(li, GL_SPOT_DIRECTION, pos);
                ++li;
                }
            //qWarning("nombre lumieres = %d", li - GL_LIGHT0);
            */
            glMatrixMode(GL_MODELVIEW);
            for (Lib3dsNode* p=file->nodes; p!=NULL; p=p->next)
                render(p);
        glEndList();
        MSG_CHECK_GL;
        
        initialized = true;
        }
    }
        
void Scene3DS::render() const {
    if (displist > 0) glCallList(displist);
    }

Vec3  Scene3DS::getCenter() const { return center; }
float Scene3DS::getRadius() const { return radius; }

/***************************************************************************************************/

void Scene3DS::render(Lib3dsNode *node) const {
    for (Lib3dsNode* p=node->childs; p!=0; p=p->next)
        render(p);

    if (node->type == LIB3DS_OBJECT_NODE) {
        //MSG_DEBUG_VAR(node->name);
        
        glPushMatrix();
        Lib3dsObjectData* d = &node->data.object;
        glMultMatrixf(&node->matrix[0][0]);
        glTranslatef(-d->pivot[0], -d->pivot[1], -d->pivot[2]);

        Lib3dsMesh *mesh = lib3ds_file_mesh_by_name(file, node->name);
        if (mesh != NULL) {
            Lib3dsMatrix M;
            lib3ds_matrix_copy(M, mesh->matrix);
            lib3ds_matrix_inv(M);
            glMultMatrixf(&M[0][0]);
            
            Lib3dsVector *normalL = new Lib3dsVector[3*mesh->faces];
            lib3ds_mesh_calculate_normals(mesh, normalL);
    
            glBegin(GL_TRIANGLES);
            for (unsigned int p=0; p<mesh->faces; ++p) {
                Lib3dsFace *f = &mesh->faceL[p];
                Lib3dsMaterial *mat = lib3ds_file_material_by_name(file, f->material);
    
                Texture *tex = NULL;
                
                //if (mat != NULL && mat->texture1_map.name[0]) MSG_DEBUG_VAR(mat->texture1_map.name);
                if (mat != NULL && mat->texture1_map.name[0]) {
                    tex = static_cast<Texture*>(mat->texture1_map.user.p);
                    }
                
                const Lib3dsRgba BLACK = { 0.0, 0.0, 0.0, 1.0 };
                const Lib3dsRgba GRAY  = { 0.8, 0.8, 0.8, 1.0 };
                
                //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT  , BLACK);
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT  , mat == NULL ? BLACK : mat->ambient);
                //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE  , mat == NULL ? GRAY  : 1.6*Float4(mat->diffuse));
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE  , mat == NULL ? GRAY  : mat->diffuse);
                //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR , BLACK);
                //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE  , mat == NULL ? GRAY  : mat->specular);
                //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE  , mat == NULL ? GRAY  : 0.5 * Float4(mat->specular) + 0.5 * Float4(mat->diffuse));
                //if (mat->diffuse[2] == 0.0f)
                //    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE  , mat == NULL ? GRAY  : 0.4 * Float4(mat->specular) + 0.6 * Float4(mat->diffuse));
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR , mat == NULL ? BLACK : mat->specular);
                glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mat == NULL ? 1.0f  : fmaxf(powf(2, 10.0 * mat->shininess), 128.0f));

                //glEnable(GL_COLOR_MATERIAL);
                glColor4fv(mat == NULL ? GRAY  : mat->diffuse);
                //glColor4fv(mat == NULL ? GRAY  : 1.6*Float4(mat->diffuse));
                //glColor4fv(mat == NULL ? GRAY  : Float4(mat->diffuse) + Float4(0.2,0.2,0.2,0.0));

                /*
                // version diffuse seulement (suppose avoir fait glEnable(GL_COLOR_MATERIAL)) :
                if (mat == NULL) glColor4fv(GRAY);
                else glColor4fv(mix(Float4(mat->diffuse), Float4(mat->specular), s);
                */
                
                glActiveTextureARB(GL_TEXTURE0_ARB);
                if (tex != NULL) tex->activate();

                //glNormal3fv(f->normal);
                for (int i=0; i<3; ++i) {
                    glNormal3fv(normalL[3*p+i]);
                    glVertex3fv(mesh->pointL[f->points[i]].pos);
                    }

                if (tex != NULL) tex->deactivate();
                }
            glEnd();
    
            delete[] normalL;
            }

        glPopMatrix();
        }
    /*
    else {
        switch(node->type) {
            case LIB3DS_UNKNOWN_NODE : Message::debug("noeud de type LIB3DS_UNKNOWN_NODE"); break;
            case LIB3DS_AMBIENT_NODE : Message::debug("noeud de type LIB3DS_AMBIENT_NODE"); break;
            case LIB3DS_CAMERA_NODE  : Message::debug("noeud de type LIB3DS_CAMERA_NODE");  break;
            case LIB3DS_TARGET_NODE  : Message::debug("noeud de type LIB3DS_TARGET_NODE");  break;
            case LIB3DS_LIGHT_NODE   : Message::debug("noeud de type LIB3DS_LIGHT_NODE");   break;
            case LIB3DS_SPOT_NODE    : Message::debug("noeud de type LIB3DS_SPOT_NODE");    break;
            default:;
            }
        }
    */
    }

/***************************************************************************************************/
/*
void Chargeur3DS::calcNombreTriangles() {
    nombreTriangles = 0;
    for (Lib3dsNode* p=file->nodes; p!=0; p=p->next)
        nombreTriangles += calcNombreTriangles(p);
    }

int Chargeur3DS::calcNombreTriangles(Lib3dsNode *noeud) const {
    int res = 0;

    for (Lib3dsNode* p=noeud->childs; p!=0; p=p->next)
        res += calcNombreTriangles(p);

    if (noeud->type == LIB3DS_OBJECT_NODE && strcmp(noeud->name, "$$$DUMMY") != 0) {
        Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(file, noeud->name);
        if (mesh) res += mesh->faces;
        }
    return res;
    }
*/

