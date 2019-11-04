#include <stdio.h>
#include <stdlib.h>
#include "string.h"

typedef char* String;
typedef enum{RED,BLACK} Color;
typedef enum{FALSE,TRUE} Boolean;

struct sorgEl;
typedef struct sorgEl SorgElement;

struct RelType;
typedef struct RelType RelationType;

struct SorgN;
typedef struct SorgN SorgNode;
typedef struct {
    SorgNode* nil;
    SorgNode* root;
}SorgTree;


struct Ent{
    String name;
    SorgElement* headSorg;
    //stuff for RB tree
    Color color;
    struct Ent* father;
    struct Ent* left;
    struct Ent* right;
};

typedef struct Ent Entity;
typedef struct {
    Entity* nil;
    Entity* root;
}EntityTree;

struct CachedEnt{
    Entity* entity;
    struct CachedEnt* next;
};
typedef struct CachedEnt CachedEntity;

typedef struct{
    int maxRelDest;
    CachedEntity* maxDests;//lista destinatario con numRel=maxRelDest ordinato per Nome.
} CacheSystem;

struct Rel{
    Entity* dest;
    SorgTree sorgTree;
    RelationType* relationType;
    int count;
    //stuff for RB tree
    Color color;
    struct Rel* father;
    struct Rel* left;
    struct Rel* right;
};
typedef struct Rel RelationDest;
typedef struct {
    RelationDest* nil;
    RelationDest* root;
}RelationTree;

struct sorgEl{
    SorgNode* sorgNode;
    Entity* myEntity;
    struct sorgEl* next;
    struct sorgEl* prev;
};
typedef struct sorgEl SorgElement;

struct SorgN{
    RelationDest* relationDest;
    SorgElement* sorgElement;
    //stuff for RB tree
    Color color;
    struct SorgN* father;
    struct SorgN* left;
    struct SorgN* right;
};
typedef struct SorgN SorgNode;

struct RelType{
    String relNameType;
    RelationTree dest;//Albero delle relazioni del tipo relNameType
    //CacheSystem* myCache; //Cache che contiene il num max di relaz per questo tipo e i destinatari che le hanno.
    //stuff for RB tree
    Color color;
    struct RelType* father;
    struct RelType* left;
    struct RelType* right;
};
typedef struct RelType RelationType;
typedef struct {
    RelationType* nil;
    RelationType* root;
}RelationTypeTree;



void insertEntity(EntityTree* entityTree,String newEntityName);
void deleteEntity(EntityTree *entityTree, char *name, RelationDest *nil, RelationTypeTree *relationTypeTree);//abbastanza critica
Entity* searchEntity(Entity* nil,Entity* x, char* name);

RelationType* searchRelationType(RelationType* nil,RelationType* x, char* name);
RelationType* relationTypeMinimum(RelationType* nil, RelationType* x);
RelationType* relationTypeSuccessor(RelationTypeTree* t,RelationType* x);
void insertRelationType(RelationTypeTree* relationTypeTree,RelationType* newRelationType);
Boolean deleteRelationType(RelationTypeTree* RelationTypeTree, RelationType* delRelationType);

RelationDest* searchRelationDest(RelationDest* nil,RelationDest* x, Entity* dest);
RelationDest* insertRelation(RelationTree* relationTree,Entity* destEntity, Entity* sorgEntity, SorgNode* nil);
RelationDest* searchRelationFromDest(RelationDest *nil, RelationDest *x, Entity *dest);
RelationDest* relationPredecessor(RelationDest *nil, RelationDest *x);
Boolean deleteRelation(RelationTree* RelationTree, RelationDest* delRelation);
void deleteAllRelationFromDest(RelationTree* relationTree, Entity* dest);
void deleteSorgNodeFromSorgRel(SorgNode *delSorgNode, RelationDest *nil, RelationTypeTree *relationTypeTree);
int relationWalk(RelationDest* relation, RelationTree* relationTree);

SorgNode* searchSorgNode(SorgNode* nil,SorgNode* x, Entity* sorg);
Boolean deleteSorgNode(SorgTree* sorgTree, SorgNode* z);

void insertCachedEntity(CachedEntity** first, Entity* entToCache);
Boolean removeCachedEntity(CachedEntity **first, Entity *entity);
void releaseCache(CachedEntity**first);
int cacheContains(CachedEntity** first,Entity* entity);
void recalculateMax(RelationType* relationType);

void printReport(RelationTypeTree relationTypeTree);

int main (int argc,char*argv[]){
    //printf("Color:%ld;\n RelationDest:%ld ;\n Entity: %ld;\n RelationType: %ld\n", sizeof(Color), sizeof(RelationDest), sizeof(Entity), sizeof(RelationType));
    EntityTree* entityTree=malloc(sizeof(EntityTree));
    RelationTypeTree* relationTypeTree= malloc(sizeof(RelationTypeTree));
    char command[10];
    String entSource, entDest, relType;
    char entSourceRead[10000], entDestRead[10000], relTypeRead[1000];

    Entity* entityNil = malloc(sizeof(Entity));
    entityNil->color=BLACK;
    entityNil->name=NULL;
    entityNil->left=entityNil;
    entityNil->right=entityNil;
    entityNil->father=entityNil;
    entityTree->nil=entityNil;
    entityTree->root=entityNil;

    RelationType* relationTypeNil = malloc(sizeof(RelationType));
    relationTypeNil->relNameType=NULL;

    relationTypeNil->color=BLACK;
    relationTypeNil->father=relationTypeNil;
    relationTypeNil->left=relationTypeNil;
    relationTypeNil->right=relationTypeNil;
    relationTypeTree->nil=relationTypeNil;
    relationTypeTree->root=relationTypeNil;

    RelationDest* relationDestNil = malloc(sizeof(RelationDest));
    relationDestNil->color=BLACK;
    relationDestNil->dest=entityNil;
    relationDestNil->father=relationDestNil;
    relationDestNil->left=relationDestNil;
    relationDestNil->right=relationDestNil;
    relationDestNil->relationType=NULL;
    //relationDestNil->hashSorg=-1;

    SorgNode* sorgNodeNil = malloc(sizeof(SorgNode));
    sorgNodeNil->color=BLACK;
    sorgNodeNil->sorgElement=NULL;
    sorgNodeNil->father=sorgNodeNil;
    sorgNodeNil->left=sorgNodeNil;
    sorgNodeNil->right=sorgNodeNil;
//0x5558105c83f0

    do{
        scanf("%s",command);
        if(0==strcmp(command,"addent")) {
            scanf(" \"%s",entSourceRead);
            insertEntity(entityTree,entSourceRead);
        }
        else if(strcmp(command,"delent")==0) {
            scanf(" \"%s",entSourceRead);
            deleteEntity(entityTree,entSourceRead, relationDestNil, relationTypeTree);
        }
        else if(strcmp(command,"addrel")==0) {
            scanf(" \"%s",entSourceRead);
            scanf(" \"%s",entDestRead);
            scanf(" \"%s",relTypeRead);

            Entity* dest = searchEntity(entityTree->nil,entityTree->root,entDestRead);
            if(dest==entityTree->nil) continue;
            Entity* sorg = searchEntity(entityTree->nil,entityTree->root,entSourceRead);
            if(sorg==entityTree->nil) continue;
            //Boolean recalculate=TRUE;
            //Boolean updateCache=FALSE;
            RelationType* newRelationType = searchRelationType(relationTypeTree->nil,relationTypeTree->root,relTypeRead);

            if (newRelationType==relationTypeNil){
                newRelationType=malloc(sizeof(RelationType));
                newRelationType->relNameType=malloc(strlen(relTypeRead));
                strcpy(newRelationType->relNameType,relTypeRead);
                insertRelationType(relationTypeTree,newRelationType);
                RelationTree* relationTree =malloc(sizeof(RelationTree));
                relationTree->nil=relationDestNil;
                relationTree->root=relationDestNil;
                newRelationType->dest=*relationTree;
                
            }
            RelationDest* newRelation=insertRelation(&newRelationType->dest,dest,sorg, sorgNodeNil);
            newRelation->relationType=newRelationType;
            
        }
        else if(strcmp(command,"delrel")==0) {
            scanf(" \"%s",entSourceRead);
            scanf(" \"%s",entDestRead);
            scanf(" \"%s",relTypeRead);

            Entity* dest = searchEntity(entityTree->nil,entityTree->root,entDestRead);
            if(dest==entityTree->nil) continue;
            Entity* sorg = searchEntity(entityTree->nil,entityTree->root,entSourceRead);
            if(sorg==entityTree->nil) continue;
            RelationType* delRelationType = searchRelationType(relationTypeNil,relationTypeTree->root,relTypeRead);
            if(delRelationType==relationTypeNil)continue;
            RelationDest* delRelationDest= searchRelationDest(delRelationType->dest.nil,delRelationType->dest.root,dest);
            if(delRelationDest==delRelationType->dest.nil){
                continue;
            }
            SorgNode* delSorgNode=searchSorgNode(sorgNodeNil,delRelationDest->sorgTree.root,sorg);
            if(delSorgNode==delRelationDest->sorgTree.nil) {
                continue;
            }
            deleteSorgNode(&delRelationDest->sorgTree,delSorgNode);

            //se il relDest non ha più sorgNode, lo cancello
            if(delRelationDest->sorgTree.root==delRelationDest->sorgTree.nil){
                deleteRelation(&delRelationType->dest,delRelationDest);
            }

            //se l'entità ha come albero il relationTree->nil, cancella la relationType dall'albero
            if (delRelationType->dest.root==relationDestNil) {
                deleteRelationType(relationTypeTree, delRelationType);
            }
            
        }
        else if(strcmp(command,"report")==0) {
            if (relationTypeTree->root==relationTypeNil){
                printf("none\n");
            } else{
                printReport(*relationTypeTree);
            }
        }
        fgetc(stdin);
    }while (strcmp(command,"end")!=0);
    return 0;
}

Entity* searchEntity(Entity* nil,Entity* x, char* name){
    if (x ==nil || 0==strcmp(name,x->name)){
        return x;
    }
    if (strcmp(name,x->name)<0){
        return searchEntity(nil,x->left,name);
    } else {
        return searchEntity(nil,x->right,name);
    }
}
Entity* entityMinimum(Entity* nil, Entity* x){
    while (x->left!=nil) {
        x=x->left;
    }
    return x;
}
Entity* entitySuccessor(EntityTree* t,Entity* x){
    if (x->right!=t->nil){
        return entityMinimum(t->nil,x->right);
    }
    Entity* y=x->father;
    while (y!=t->nil && x==y->right){
        x=y;
        y=y->father;
    }
    return y;
}

void entityLeftRotate(EntityTree **entityTree, Entity *newEntity){
    Entity* y=newEntity->right;
    newEntity->right=y->left;

    if(y->left!=(*entityTree)->nil){
        y->left->father=newEntity;
    }
    y->father=newEntity->father;

    if (newEntity->father==(*entityTree)->nil){
        (*entityTree)->root=y;
    } else if (newEntity == newEntity->father->left){
        newEntity->father->left=y;
    }else {
        newEntity->father->right = y;
    }
     y->left = newEntity;
     newEntity->father = y;
}
void entityRightRotate(EntityTree **entityTree, Entity *newEntity){
    Entity* x=newEntity->left;
    newEntity->left=x->right;
    if(x->right!=(*entityTree)->nil){
        x->right->father=newEntity;
    }
    x->father=newEntity->father;

    if (newEntity->father==(*entityTree)->nil){
        (*entityTree)->root=x;
    } else if (newEntity==newEntity->father->right){
        newEntity->father->right=x;
    } else newEntity->father->left=x;
    x->right=newEntity;
    newEntity->father=x;
}

void insertEntityFixup(EntityTree** entityTree, Entity* z){
    if (z==(*entityTree)->root){
        ((*entityTree)->root)->color=BLACK;
    } else{
        Entity* x = z->father;
        if (x->color==RED){
            if (x==x->father->left) {
                Entity* y = x->father->right;
                if (y->color==RED){
                    x->color=BLACK;
                    y->color=BLACK;
                    x->father->color=RED;
                    insertEntityFixup(entityTree,x->father);
                } else {
                    if (z == x->right) {
                        z = x;
                        entityLeftRotate(entityTree, z);
                        x = z->father;
                    }

                    x->color = BLACK;
                    x->father->color = RED;
                    entityRightRotate(entityTree, x->father);
                }

            } else {
                Entity* y = x->father->left;
                if (y->color==RED){
                    x->color=BLACK;
                    y->color=BLACK;
                    x->father->color=RED;
                    insertEntityFixup(entityTree,x->father);
                } else {
                    if (z == x->left) {
                        z = x;
                        entityRightRotate(entityTree, z);
                        x = z->father;
                    }

                    x->color = BLACK;
                    x->father->color = RED;
                    entityLeftRotate(entityTree, x->father);
                }
            }

        }
    }
}
void insertEntity(EntityTree* entityTree,String newEntityName){
    Entity* y=entityTree->nil;
    Entity* x=(entityTree)->root;
    while (x!=entityTree->nil) {
        y = x;
        if (strcmp(newEntityName, x->name) == 0){
            //free(newEntity);
            return;
        }
        if (strcmp(newEntityName, x->name) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    Entity* newEntity = malloc(sizeof(Entity));
    newEntity->name=malloc(strlen(newEntityName)+1);
    strcpy(newEntity->name,newEntityName);
    newEntity->father=y;
    if (y==entityTree->nil){
        (entityTree)->root=newEntity;
    } else if (strcmp(newEntity->name,y->name)<0){
        y->left=newEntity;
    } else {
        y->right=newEntity;
    }
    newEntity->left=entityTree->nil;
    newEntity->right=entityTree->nil;
    newEntity->color=RED;
    newEntity->headSorg=NULL;
    insertEntityFixup(&entityTree,newEntity);
}

void entityTransplant(EntityTree** entityTree, Entity* u, Entity* v){
    if(u->father==(*entityTree)->nil){
        (*entityTree)->root=v;
    } else if(u==u->father->left){
        u->father->left=v;
    } else {
        u->father->right=v;
    }
    v->father=u->father;
}

void deleteEntityFixup(EntityTree* entityTree, Entity* x){
    Entity* w;
    while (x!=entityTree->root && x->color==BLACK){
        if(x==x->father->left){
            w=x->father->right;
            if(w->color==RED){
                w->color=BLACK;
                x->father->color=RED;
                entityLeftRotate(&entityTree,x->father);
                w=x->father->right;
            }
            if(w->left->color==BLACK && w->right->color==BLACK){
                w->color=RED;
                x=x->father;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    entityRightRotate(&entityTree, w);
                    w = x->father->right;
                }
                w->color = x->father->color;
                x->father->color = BLACK;
                w->right->color = BLACK;
                entityLeftRotate(&entityTree, x->father);
                x = entityTree->root;
            }
        } else{
            w=x->father->left;
            if(w->color==RED){
                w->color=BLACK;
                x->father->color=RED;
                entityRightRotate(&entityTree,x->father);
                w=x->father->left;
            }
            if(w->right->color==BLACK && w->left->color==BLACK){
                w->color=RED;
                x=x->father;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    entityLeftRotate(&entityTree, w);
                    w = x->father->left;
                }
                w->color = x->father->color;
                x->father->color = BLACK;
                w->left->color = BLACK;
                entityRightRotate(&entityTree, x->father);
                x = entityTree->root;
            }
        }
    }
    x->color=BLACK;
}
void deleteEntity(EntityTree *entityTree, char *name, RelationDest *nil, RelationTypeTree *relationTypeTree){
    Entity* delEntity = searchEntity(entityTree->nil,entityTree->root,name);

    if (delEntity==entityTree->nil) {
        return;
    }

    //cancella tutte le relazioni di cui sono sorgente
    SorgElement* sorgElement= delEntity->headSorg;
    while (sorgElement!=NULL){
        SorgElement* del = sorgElement;
        sorgElement=sorgElement->next;
        deleteSorgNodeFromSorgRel(del->sorgNode, nil, relationTypeTree);
    }

    if(relationTypeTree->nil!=relationTypeTree->root) {
        //cancella tutte le relazioni di cui sono destinatario
        RelationType *relationType = relationTypeMinimum(relationTypeTree->nil, relationTypeTree->root);
        while (relationType != relationTypeTree->nil) {
            deleteAllRelationFromDest(&relationType->dest, delEntity);
            RelationType *prec = relationType;
            relationType = relationTypeSuccessor(relationTypeTree, relationType);
            if (prec->dest.nil == prec->dest.root) {
                deleteRelationType(relationTypeTree, prec);
            
    }
    Entity *y,*x;
    y=delEntity;
    Color y_original_color= y->color;
    if(delEntity->left==entityTree->nil){
        x=delEntity->right;
        entityTransplant(&entityTree,delEntity,delEntity->right);
    } else if(delEntity->right==entityTree->nil){
        x=delEntity->left;
        entityTransplant(&entityTree,delEntity,delEntity->left);
    } else {
        y=entityMinimum(entityTree->nil,delEntity->right);
        y_original_color=y->color;
        x=y->right;
        if(y->father==delEntity){
            x->father=y;
        } else {
            entityTransplant(&entityTree,y,y->right);
            y->right=delEntity->right;
            y->right->father=y;
        }
        entityTransplant(&entityTree,delEntity,y);
        y->left=delEntity->left;
        y->left->father=y;
        y->color=delEntity->color;

    }
    if (y_original_color==BLACK && x!=entityTree->nil){
        deleteEntityFixup(entityTree,x);
    }
    free(delEntity);
    delEntity=NULL;

}

RelationType* searchRelationType(RelationType* nil,RelationType* x, char* name){
    if (x ==nil || 0==strcmp(name,x->relNameType)){
        return x;
    }
    if (strcmp(name,x->relNameType)<0){
        return searchRelationType(nil,x->left,name);
    } else {
        return searchRelationType(nil,x->right,name);
    }
}

RelationType* searchRelationTypeByRoot(RelationTypeTree* x, RelationTree* dest){
    RelationType* actualRelationType=relationTypeMinimum(x->nil,x->root);
    while (actualRelationType!=x->nil){
        if(actualRelationType->dest.root==dest->root) {
            return actualRelationType;
        }
        actualRelationType=relationTypeSuccessor(x,actualRelationType);
    }
}

RelationType* relationTypeMinimum(RelationType* nil, RelationType* x){
    while (x->left!=nil) {
        x=x->left;
    }
    return x;
}
RelationType* relationTypeSuccessor(RelationTypeTree* t,RelationType* x){
    if (x->right!=t->nil){
        return relationTypeMinimum(t->nil,x->right);
    }
    RelationType* y=x->father;
    while (y!=t->nil && x==y->right){
        x=y;
        y=y->father;
    }
    return y;
}

void relationTypeLeftRotate(RelationTypeTree **relationTypeTree, RelationType *newRelationType){
    RelationType* y=newRelationType->right;
    newRelationType->right=y->left;

    if(y->left!=(*relationTypeTree)->nil){
        y->left->father=newRelationType;
    }
    y->father=newRelationType->father;

    if (newRelationType->father==(*relationTypeTree)->nil){
        (*relationTypeTree)->root=y;
    } else if (newRelationType == newRelationType->father->left){
        newRelationType->father->left=y;
    }else {
        newRelationType->father->right = y;
    }
    y->left = newRelationType;
    newRelationType->father = y;
}
void relationTypeRightRotate(RelationTypeTree **relationTypeTree, RelationType *newRelationType){
    RelationType* x=newRelationType->left;
    newRelationType->left=x->right;
    if(x->right!=(*relationTypeTree)->nil){
        x->right->father=newRelationType;
    }
    x->father=newRelationType->father;

    if (newRelationType->father==(*relationTypeTree)->nil){
        (*relationTypeTree)->root=x;
    } else if (newRelationType==newRelationType->father->right){
        newRelationType->father->right=x;
    } else newRelationType->father->left=x;
    x->right=newRelationType;
    newRelationType->father=x;
}

void insertRelationTypeFixup(RelationTypeTree** relationTypeTree, RelationType* z){
    if (z==(*relationTypeTree)->root){
        ((*relationTypeTree)->root)->color=BLACK;
    } else{
        RelationType* x = z->father;
        if (x->color==RED){
            if (x==x->father->left) {
                RelationType* y = x->father->right;
                if (y->color==RED){
                    x->color=BLACK;
                    y->color=BLACK;
                    x->father->color=RED;
                    insertRelationTypeFixup(relationTypeTree,x->father);
                } else {
                    if (z == x->right) {
                        z = x;
                        relationTypeLeftRotate(relationTypeTree, z);
                        x = z->father;
                    }

                    x->color = BLACK;
                    x->father->color = RED;
                    relationTypeRightRotate(relationTypeTree, x->father);
                }

            } else {
                RelationType* y = x->father->left;
                if (y->color==RED){
                    x->color=BLACK;
                    y->color=BLACK;
                    x->father->color=RED;
                    insertRelationTypeFixup(relationTypeTree,x->father);
                } else {
                    if (z == x->left) {
                        z = x;
                        relationTypeRightRotate(relationTypeTree, z);
                        x = z->father;
                    }

                    x->color = BLACK;
                    x->father->color = RED;
                    relationTypeLeftRotate(relationTypeTree, x->father);
                }
            }

        }
    }
}
void insertRelationType(RelationTypeTree* relationTypeTree,RelationType* newRelationType){
    RelationType* y=relationTypeTree->nil;
    RelationType* x=(relationTypeTree)->root;
    while (x!=relationTypeTree->nil) {
        y = x;
        if (strcmp(newRelationType->relNameType, x->relNameType) == 0){
            return;
        }
        if (strcmp(newRelationType->relNameType, x->relNameType) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    newRelationType->father=y;
    if (y==relationTypeTree->nil){
        (relationTypeTree)->root=newRelationType;
    } else if (strcmp(newRelationType->relNameType,y->relNameType)<0){
        y->left=newRelationType;
    } else {
        y->right=newRelationType;
    }
    newRelationType->left=relationTypeTree->nil;
    newRelationType->right=relationTypeTree->nil;
    newRelationType->color=RED;
    insertRelationTypeFixup(&relationTypeTree,newRelationType);

}

void relationTypeTransplant(RelationTypeTree** entityTree, RelationType* u, RelationType* v){
    if(u->father==(*entityTree)->nil){
        (*entityTree)->root=v;
    } else if(u==u->father->left){
        u->father->left=v;
    } else {
        u->father->right=v;
    }
    v->father=u->father;
}

void deleteRelationTypeFixup(RelationTypeTree* relationTypeTree, RelationType* x){
    RelationType* w;
    while (x!=relationTypeTree->root && x->color==BLACK){
        if(x==x->father->left){
            w=x->father->right;
            if(w->color==RED){
                w->color=BLACK;
                x->father->color=RED;
                relationTypeLeftRotate(&relationTypeTree,x->father);
                w=x->father->right;
            }
            if(w->left->color==BLACK && w->right->color==BLACK){
                w->color=RED;
                x=x->father;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    relationTypeRightRotate(&relationTypeTree, w);
                    w = x->father->right;
                }
                w->color = x->father->color;
                x->father->color = BLACK;
                w->right->color = BLACK;
                relationTypeLeftRotate(&relationTypeTree, x->father);
                x = relationTypeTree->root;
            }
        } else{
            w=x->father->left;
            if(w->color==RED){
                w->color=BLACK;
                x->father->color=RED;
                relationTypeRightRotate(&relationTypeTree,x->father);
                w=x->father->left;
            }
            if(w->right->color==BLACK && w->left->color==BLACK){
                w->color=RED;
                x=x->father;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    relationTypeLeftRotate(&relationTypeTree, w);
                    w = x->father->left;
                }
                w->color = x->father->color;
                x->father->color = BLACK;
                w->left->color = BLACK;
                relationTypeRightRotate(&relationTypeTree, x->father);
                x = relationTypeTree->root;
            }
        }
    }
    x->color=BLACK;

}
Boolean deleteRelationType(RelationTypeTree* relationTypeTree, RelationType* z){
    if (z==relationTypeTree->nil) {
        return FALSE;
    }
    RelationType *y,*x;
    y=z;
    Color y_original_color= y->color;
    if(z->left==relationTypeTree->nil){
        x=z->right;
        relationTypeTransplant(&relationTypeTree,z,z->right);
    } else if(z->right==relationTypeTree->nil){
        x=z->left;
        relationTypeTransplant(&relationTypeTree,z,z->left);
    } else {
        y=relationTypeMinimum(relationTypeTree->nil,z->right);
        y_original_color=y->color;
        x=y->right;
        if(y->father==z){
            x->father=y;
        } else {
            relationTypeTransplant(&relationTypeTree,y,y->right);
            y->right=z->right;
            y->right->father=y;
        }
        relationTypeTransplant(&relationTypeTree,z,y);
        y->left=z->left;
        y->left->father=y;
        y->color=z->color;

    }

    if (y_original_color==BLACK && x!=relationTypeTree->nil){
        deleteRelationTypeFixup(relationTypeTree,x);
    }

    free(z);
    z=NULL;
    return TRUE;

}





SorgNode* searchSorgNode(SorgNode* nil,SorgNode* x, Entity* sorg){
    if (x ==nil || sorg==x->sorgElement->myEntity){
        return x;
    }
    if (strcmp(sorg->name,x->sorgElement->myEntity->name)<0){
        return searchSorgNode(nil,x->left,sorg);
    } else {
        return searchSorgNode(nil,x->right,sorg);
    }
}

SorgNode* sorgNodeMinimum(SorgNode* nil, SorgNode* x){
    while (x->left!=nil) {
        x=x->left;
    }
    return x;
}
SorgNode* sorgNodeSuccessor(SorgTree* t,SorgNode* x){
    if (x->right!=t->nil){
        return sorgNodeMinimum(t->nil,x->right);
    }
    SorgNode* y=x->father;
    while (y!=t->nil && x==y->right){
        x=y;
        y=y->father;
    }
    return y;
}

void sorgNodeLeftRotate(SorgTree **sorgTree, SorgNode *newSorgNode){
    SorgNode* y=newSorgNode->right;
    newSorgNode->right=y->left;

    if(y->left!=(*sorgTree)->nil){
        y->left->father=newSorgNode;
    }
    y->father=newSorgNode->father;

    if (newSorgNode->father==(*sorgTree)->nil){
        (*sorgTree)->root=y;
    } else if (newSorgNode == newSorgNode->father->left){
        newSorgNode->father->left=y;
    }else {
        newSorgNode->father->right = y;
    }
    y->left = newSorgNode;
    newSorgNode->father = y;
}
void sorgNodeRightRotate(SorgTree **sorgTree, SorgNode *newSorgNode){
    SorgNode* x=newSorgNode->left;
    newSorgNode->left=x->right;
    if(x->right!=(*sorgTree)->nil){
        x->right->father=newSorgNode;
    }
    x->father=newSorgNode->father;

    if (newSorgNode->father==(*sorgTree)->nil){
        (*sorgTree)->root=x;
    } else if (newSorgNode==newSorgNode->father->right){
        newSorgNode->father->right=x;
    } else newSorgNode->father->left=x;
    x->right=newSorgNode;
    newSorgNode->father=x;
}

void insertSorgNodeFixup(SorgTree** sorgTree, SorgNode* z){
    if (z==(*sorgTree)->root){
        ((*sorgTree)->root)->color=BLACK;
    } else{
        SorgNode* x = z->father;
        if (x->color==RED){
            if (x==x->father->left) {
                SorgNode* y = x->father->right;
                if (y->color==RED){
                    x->color=BLACK;
                    y->color=BLACK;
                    x->father->color=RED;
                    insertSorgNodeFixup(sorgTree,x->father);
                } else {
                    if (z == x->right) {
                        z = x;
                        sorgNodeLeftRotate(sorgTree, z);
                        x = z->father;
                    }

                    x->color = BLACK;
                    x->father->color = RED;
                    sorgNodeRightRotate(sorgTree, x->father);
                }

            } else {
                SorgNode* y = x->father->left;
                if (y->color==RED){
                    x->color=BLACK;
                    y->color=BLACK;
                    x->father->color=RED;
                    insertSorgNodeFixup(sorgTree,x->father);
                } else {
                    if (z == x->left) {
                        z = x;
                        sorgNodeRightRotate(sorgTree, z);
                        x = z->father;
                    }

                    x->color = BLACK;
                    x->father->color = RED;
                    sorgNodeLeftRotate(sorgTree, x->father);
                }
            }

        }
    }
}
void insertSorgNode(SorgTree* sorgTree,Entity* sorgEntity, RelationDest* dest){
    SorgNode* y=sorgTree->nil;
    SorgNode* x=(sorgTree)->root;
    while (x!=sorgTree->nil) {
        y = x;
        if (sorgEntity== x->sorgElement->myEntity){
            return;
        }
        if (strcmp(sorgEntity->name, x->sorgElement->myEntity->name) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    SorgNode* newSorgNode= malloc(sizeof(SorgNode));

    //Creazione elemento lista
    SorgElement* newSorgElement = malloc(sizeof(SorgElement));

    newSorgElement->myEntity=sorgEntity;
    newSorgElement->prev=NULL;
    newSorgElement->next=sorgEntity->headSorg;
    sorgEntity->headSorg=newSorgElement;
    if(newSorgElement->next!=NULL){
        newSorgElement->next->prev=newSorgElement;
    }
    newSorgNode->relationDest=dest;
    newSorgNode->sorgElement=newSorgElement;
    newSorgNode->father=y;
    if (y==sorgTree->nil){
        (sorgTree)->root=newSorgNode;
    } else if (strcmp(newSorgNode->sorgElement->myEntity->name,y->sorgElement->myEntity->name)<0){
        y->left=newSorgNode;
    } else {
        y->right=newSorgNode;
    }
    newSorgNode->left=sorgTree->nil;
    newSorgNode->right=sorgTree->nil;
    newSorgNode->color=RED;
    insertSorgNodeFixup(&sorgTree,newSorgNode);
    newSorgElement->sorgNode=newSorgNode;
    //Incremento il count del RelationDest di 1 pperchè ho inserito un nuovo SorgNode
    dest->count+=1;
}

void sorgNodeTransplant(SorgTree** sorgTree, SorgNode* u, SorgNode* v){
    if(u->father==(*sorgTree)->nil){
        (*sorgTree)->root=v;
    } else if(u==u->father->left){
        u->father->left=v;
    } else {
        u->father->right=v;
    }
    v->father=u->father;
}

void deleteSorgNodeFixup(SorgTree* sorgTree, SorgNode* x){
    SorgNode* w;
    while (x!=sorgTree->root && x->color==BLACK){
        if(x==x->father->left){
            w=x->father->right;
            if(w->color==RED){
                w->color=BLACK;
                x->father->color=RED;
                sorgNodeLeftRotate(&sorgTree,x->father);
                w=x->father->right;
            }
            if(w->left->color==BLACK && w->right->color==BLACK){
                w->color=RED;
                x=x->father;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    sorgNodeRightRotate(&sorgTree, w);
                    w = x->father->right;
                }
                w->color = x->father->color;
                x->father->color = BLACK;
                w->right->color = BLACK;
                sorgNodeLeftRotate(&sorgTree, x->father);
                x = sorgTree->root;
            }
        } else{
            w=x->father->left;
            if(w->color==RED){
                w->color=BLACK;
                x->father->color=RED;
                sorgNodeRightRotate(&sorgTree,x->father);
                w=x->father->left;
            }
            if(w->right->color==BLACK && w->left->color==BLACK){
                w->color=RED;
                x=x->father;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    sorgNodeLeftRotate(&sorgTree, w);
                    w = x->father->left;
                }
                w->color = x->father->color;
                x->father->color = BLACK;
                w->left->color = BLACK;
                sorgNodeRightRotate(&sorgTree, x->father);
                x = sorgTree->root;
            }
        }
    }
    x->color=BLACK;

}
Boolean deleteSorgNode(SorgTree* sorgTree, SorgNode* z){
    if (z==sorgTree->nil) {
        return FALSE;
    }
    SorgNode *y,*x;
    y=z;
    Color y_original_color= y->color;
    if(z->left==sorgTree->nil){
        x=z->right;
        sorgNodeTransplant(&sorgTree,z,z->right);
    } else if(z->right==sorgTree->nil){
        x=z->left;
        sorgNodeTransplant(&sorgTree,z,z->left);
    } else {
        y=sorgNodeMinimum(sorgTree->nil,z->right);
        y_original_color=y->color;
        x=y->right;
        if(y->father==z){
            x->father=y;
        } else {
            sorgNodeTransplant(&sorgTree,y,y->right);
            y->right=z->right;
            y->right->father=y;
        }
        sorgNodeTransplant(&sorgTree,z,y);
        y->left=z->left;
        y->left->father=y;
        y->color=z->color;

    }

    if (y_original_color==BLACK && x!=sorgTree->nil){
        deleteSorgNodeFixup(sorgTree,x);
    }
    if(z->sorgElement->prev!=NULL){
        z->sorgElement->prev->next=z->sorgElement->next;
    } else {
        z->sorgElement->myEntity->headSorg=z->sorgElement->next;
    }
    if(z->sorgElement->next!=NULL){
        z->sorgElement->next->prev=z->sorgElement->prev;
    }

    z->relationDest->count-=1;
    free(z->sorgElement);
    free(z);
    z=NULL;
    return TRUE;

}








RelationDest* searchRelationDest(RelationDest* nil,RelationDest* x, Entity* dest){
    if (x ==nil || (x->dest==dest)){
        return x;
    }
    if (strcmp(dest->name,x->dest->name)<0){
        return searchRelationDest(nil,x->left,dest);
    } else if (strcmp(dest->name,x->dest->name)>0){
        return searchRelationDest(nil,x->right,dest);
    }
}

RelationDest* searchRelationFromDest(RelationDest *nil, RelationDest *x, Entity *dest){
    if (x == nil || (x->dest==dest)){
        return x;
    }
    if (strcmp(dest->name,x->dest->name)<0){
        return searchRelationFromDest(nil, x->left, dest);
    } else if (strcmp(dest->name,x->dest->name)>0){
        return searchRelationFromDest(nil, x->right, dest);
    }
}

RelationDest* relationMaximum(RelationDest* nil, RelationDest* x){
    while (x->right!=nil) {
        x=x->right;
    }
    return x;
}
RelationDest* relationMinimum(RelationDest* nil, RelationDest* x){
    while (x->left!=nil) {
        x=x->left;
    }
    return x;
}
RelationDest* relationSuccessor(RelationDest *nil, RelationDest *x){
    if (x->right!=nil){
        return relationMinimum(nil,x->right);
    }
    RelationDest* y=x->father;
    while (y!=nil && x==y->right){
        x=y;
        y=y->father;
    }
    return y;
}
RelationDest* relationPredecessor(RelationDest *nil, RelationDest *x){
    if (x->left!=nil){
        return relationMaximum(nil,x->left);
    }
    RelationDest* y=x->father;
    while (y!=nil && x==y->left){
        x=y;
        y=y->father;
    }
    return y;
}

void relationLeftRotate(RelationTree **relationTree, RelationDest *newRelation){
    RelationDest* y=newRelation->right;
    newRelation->right=y->left;

    if(y->left!=(*relationTree)->nil){
        y->left->father=newRelation;
    }
    y->father=newRelation->father;

    if (newRelation->father==(*relationTree)->nil){
        (*relationTree)->root=y;
    } else if (newRelation == newRelation->father->left){
        newRelation->father->left=y;
    }else {
        newRelation->father->right = y;
    }
    y->left = newRelation;
    newRelation->father = y;
}
void relationRightRotate(RelationTree **relationTree, RelationDest *newRelation){
    RelationDest* x=newRelation->left;
    newRelation->left=x->right;
    if(x->right!=(*relationTree)->nil){
        x->right->father=newRelation;
    }
    x->father=newRelation->father;

    if (newRelation->father==(*relationTree)->nil){
        (*relationTree)->root=x;
    } else if (newRelation==newRelation->father->right){
        newRelation->father->right=x;
    } else newRelation->father->left=x;
    x->right=newRelation;
    newRelation->father=x;
}

void insertRelationFixup(RelationTree** relationTree, RelationDest* z){
    if (z==(*relationTree)->root){
        ((*relationTree)->root)->color=BLACK;
    } else{
        RelationDest* x = z->father;
        if (x->color==RED){
            if (x==x->father->left) {
                RelationDest* y = x->father->right;
                if (y->color==RED){
                    x->color=BLACK;
                    y->color=BLACK;
                    x->father->color=RED;
                    insertRelationFixup(relationTree,x->father);
                } else {
                    if (z == x->right) {
                        z = x;
                        relationLeftRotate(relationTree, z);
                        x = z->father;
                    }

                    x->color = BLACK;
                    x->father->color = RED;
                    relationRightRotate(relationTree, x->father);
                }

            } else {
                RelationDest* y = x->father->left;
                if (y->color==RED){
                    x->color=BLACK;
                    y->color=BLACK;
                    x->father->color=RED;
                    insertRelationFixup(relationTree,x->father);
                } else {
                    if (z == x->left) {
                        z = x;
                        relationRightRotate(relationTree, z);
                        x = z->father;
                    }

                    x->color = BLACK;
                    x->father->color = RED;
                    relationLeftRotate(relationTree, x->father);
                }
            }

        }
    }
}
RelationDest* insertRelation(RelationTree* relationTree,Entity* destEntity, Entity* sorgEntity, SorgNode* nil){
    RelationDest* y=relationTree->nil;
    RelationDest* x=(relationTree)->root;
    while (x!=relationTree->nil) {
        y = x;
        if (strcmp(destEntity->name, x->dest->name) == 0){
            //Inserisci il SorgNode nell'albero di x
            insertSorgNode(&x->sorgTree,sorgEntity,x);

            return x;
        }
        if (strcmp(destEntity->name, x->dest->name) < 0)  {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    //Dest non è tra le relazioni esistenti-> Creazione di RelationDest
    RelationDest* newRelation=malloc(sizeof(RelationDest));
    newRelation->dest=destEntity;
    newRelation->count=0;
    newRelation->father=y;
    if (y==relationTree->nil){
        (relationTree)->root=newRelation;
    } else if (strcmp(newRelation->dest->name,y->dest->name)<0){
        y->left=newRelation;
    } else {
        y->right=newRelation;
    }
    newRelation->left=relationTree->nil;
    newRelation->right=relationTree->nil;
    newRelation->color=RED;
    insertRelationFixup(&relationTree,newRelation);

    //Creazione albero SorgTree
    SorgTree* newSorgTree=malloc(sizeof(SorgTree));
    newSorgTree->nil=nil;
    newSorgTree->root=nil;
    newRelation->sorgTree=*newSorgTree;

    //Inserisco SorgNode nell'albero di newRelation
    insertSorgNode(&newRelation->sorgTree,sorgEntity,newRelation);
    return newRelation;
}

int relationWalk(RelationDest* relation, RelationTree* relationTree){
    int count=1;
    RelationDest* leftRel=relationPredecessor(relationTree->nil,relation);
    RelationDest* rightRel=relationSuccessor(relationTree->nil,relation);
    while(leftRel!=relationTree->nil){
        if(strcmp(relation->dest->name,leftRel->dest->name)==0) {
            count++;
        } else{
            break;
        }
        leftRel=relationPredecessor(relationTree->nil,leftRel);
    }
    while(rightRel!=relationTree->nil){
        if(strcmp(relation->dest->name,rightRel->dest->name)==0) {
            count++;
        } else{
            break;
        }
        rightRel=relationSuccessor(relationTree->nil,rightRel);
    }
    return count;
}

void relationTransplant(RelationTree** relationTree, RelationDest* u, RelationDest* v){
    if(u->father==(*relationTree)->nil){
        (*relationTree)->root=v;
    } else if(u==u->father->left){
        u->father->left=v;
    } else {
        u->father->right=v;
    }
    v->father=u->father;
}

void deleteRelationFixup(RelationTree* relationTree, RelationDest* x){
    RelationDest* w;
    while (x!=relationTree->root && x->color==BLACK){
        if(x==x->father->left){
            w=x->father->right;
            if(w->color==RED){
                w->color=BLACK;
                x->father->color=RED;
                relationLeftRotate(&relationTree,x->father);
                w=x->father->right;
            }
            if(w->left->color==BLACK && w->right->color==BLACK){
                w->color=RED;
                x=x->father;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    relationRightRotate(&relationTree, w);
                    w = x->father->right;
                }
                w->color = x->father->color;
                x->father->color = BLACK;
                w->right->color = BLACK;
                relationLeftRotate(&relationTree, x->father);
                x = relationTree->root;
            }
        } else{
            w=x->father->left;
            if(w->color==RED){
                w->color=BLACK;
                x->father->color=RED;
                relationRightRotate(&relationTree,x->father);
                w=x->father->left;
            }
            if(w->right->color==BLACK && w->left->color==BLACK){
                w->color=RED;
                x=x->father;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    relationLeftRotate(&relationTree, w);
                    w = x->father->left;
                }
                w->color = x->father->color;
                x->father->color = BLACK;
                w->left->color = BLACK;
                relationRightRotate(&relationTree, x->father);
                x = relationTree->root;
            }
        }
    }
    x->color=BLACK;

}
Boolean deleteRelation(RelationTree* relationTree, RelationDest* z){
    if (z==relationTree->nil) {
        return FALSE;
    }
    RelationDest *y,*x;
    y=z;
    Color y_original_color= y->color;
    if(z->left==relationTree->nil){
        x=z->right;
        relationTransplant(&relationTree,z,z->right);
    } else if(z->right==relationTree->nil){
        x=z->left;
        relationTransplant(&relationTree,z,z->left);
    } else {
        y=relationMinimum(relationTree->nil,z->right);
        y_original_color=y->color;
        x=y->right;
        if(y->father==z){
            x->father=y;
        } else {
            relationTransplant(&relationTree,y,y->right);
            y->right=z->right;
            y->right->father=y;
        }
        relationTransplant(&relationTree,z,y);
        y->left=z->left;
        y->left->father=y;
        y->color=z->color;

    }

    if (y_original_color==BLACK && x!=relationTree->nil){
        deleteRelationFixup(relationTree,x);
    }
    free(z);
    z=NULL;
    return TRUE;

}
void releaseSorgTree(SorgNode* delSorgNode, SorgNode* nil){
    if(delSorgNode->left!=nil){
        releaseSorgTree(delSorgNode->left,nil);
        if(delSorgNode->left->sorgElement->prev!=NULL){
            delSorgNode->left->sorgElement->prev->next=delSorgNode->left->sorgElement->next;
        } else {
            delSorgNode->left->sorgElement->myEntity->headSorg=delSorgNode->left->sorgElement->next;
        }
        if(delSorgNode->left->sorgElement->next!=NULL){
            delSorgNode->left->sorgElement->next->prev=delSorgNode->left->sorgElement->prev;
        }
        free(delSorgNode->left->sorgElement);
        free(delSorgNode->left);
    }
    if(delSorgNode->right!=nil){
        releaseSorgTree(delSorgNode->right,nil);
        if(delSorgNode->right->sorgElement->prev!=NULL){
            delSorgNode->right->sorgElement->prev->next=delSorgNode->right->sorgElement->next;
        } else {
            delSorgNode->right->sorgElement->myEntity->headSorg=delSorgNode->right->sorgElement->next;
        }
        if(delSorgNode->right->sorgElement->next!=NULL){
            delSorgNode->right->sorgElement->next->prev=delSorgNode->right->sorgElement->prev;
        }
        free(delSorgNode->right->sorgElement);
        free(delSorgNode->right);
    }
}
void deleteAllRelationFromDest(RelationTree* relationTree, Entity* dest){
    RelationDest* delRel=searchRelationFromDest(relationTree->nil,relationTree->root,dest);
    if(delRel==relationTree->nil){
        return;
    }
    SorgNode* x=delRel->sorgTree.root;
    if(x!=delRel->sorgTree.nil) {
        releaseSorgTree(x, delRel->sorgTree.nil);
        if (x->sorgElement->prev != NULL) {
            x->sorgElement->prev->next = x->sorgElement->next;
        } else {
            x->sorgElement->myEntity->headSorg = x->sorgElement->next;
        }
        if (x->sorgElement->next != NULL) {
            x->sorgElement->next->prev = x->sorgElement->prev;
        }
        free(x->sorgElement);
        free(x);
    }
    delRel->sorgTree.root=delRel->sorgTree.nil;
    deleteRelation(relationTree,delRel);
}

void deleteSorgNodeFromSorgRel(SorgNode *delSorgNode, RelationDest *nil, RelationTypeTree *relationTypeTree){
    RelationDest* delRelDest=delSorgNode->relationDest;
    deleteSorgNode(&delSorgNode->relationDest->sorgTree,delSorgNode);
    if(delRelDest->count==0) {
        RelationType *relationTypeToUpdate = delRelDest->relationType;
        deleteRelation(&relationTypeToUpdate->dest, delRelDest);
        if (relationTypeToUpdate->dest.root==relationTypeToUpdate->dest.nil){
            deleteRelationType(relationTypeTree,relationTypeToUpdate);
            /*} else {
                if (removeCachedEntity(&relationTypeToUpdate->myCache->maxDests, dest)) {
                    if (relationTypeToUpdate->myCache->maxDests == NULL) {
                        recalculateMax(relationTypeToUpdate);
                    }
                }*/
        }
    }

}
void releaseCache(CachedEntity** first){
    CachedEntity* x=*first;
    while (x!=NULL){
        CachedEntity* del = x;
        x=x->next;
        free(del);
    }
    *first=NULL;
}
int cacheContains(CachedEntity** first,Entity* entity){
    CachedEntity* x= *first;
    while (x!=NULL && strcmp(x->entity->name,entity->name)<=0){
        if(x->entity==entity) {
            return 1;
        }
        x=x->next;
    }
    return 0;
}
void insertCachedEntity(CachedEntity** first, Entity* entToCache){
    CachedEntity* x=*first;
    CachedEntity* prec=NULL;
    if(x==NULL){
        x=malloc(sizeof(CachedEntity));
        x->entity=entToCache;
        x->next=NULL;
        *first=x;
        return;
    }
    while (x!=NULL){
        if (strcmp(x->entity->name,entToCache->name)>0){
            CachedEntity* temp=malloc(sizeof(CachedEntity));
            if(prec!=NULL){
                prec->next=temp;
            } else{
                *first=temp;
            }
            temp->next=x;
            temp->entity=entToCache;
            return;
        }
        if(x->next==NULL){
            CachedEntity* temp=malloc(sizeof(CachedEntity));
            x->next=temp;
            temp->next=NULL;
            temp->entity=entToCache;
            return;
        }
        prec=x;
        x=x->next;
    }

}

Boolean removeCachedEntity(CachedEntity **first, Entity *entity){
    CachedEntity* x= *first;
    CachedEntity* prec=NULL;
    while (x!=NULL && strcmp(x->entity->name,entity->name)<=0){
        if (x->entity==entity){
            CachedEntity* del = x;
            if(prec!=NULL){
                prec->next=x->next;
            }
            else{
                *first=x->next;
            }

            free(del);
            return TRUE;
        }
        prec=x;
        x=x->next;
    }
    return FALSE;
}

void recalculateMax(RelationType* relationType){
    RelationDest* actualRelation=relationMinimum(relationType->dest.nil,relationType->dest.root);
    RelationDest* nextRelation=relationSuccessor(relationType->dest.nil,actualRelation);
    int max=1;
    int count=1;
}

void printRelType(RelationType relationType){
    fputc('\"',stdout);
    fputs(relationType.relNameType,stdout);
    RelationDest* x=relationMinimum(relationType.dest.nil,relationType.dest.root);
    int max=0;
    CachedEntity* newCachedEntity=NULL;
    CachedEntity* first=NULL;
    while (x!=relationType.dest.nil){
        if(x->count>max){
            if(first!=NULL){
                CachedEntity* del=first;
                CachedEntity* nextCache=first->next;
                free(del);
                while (nextCache!=NULL){
                    del=nextCache;
                    free(del);
                    nextCache=nextCache->next;
                }
            }
            max=x->count;
            newCachedEntity=malloc(sizeof(CachedEntity));
            newCachedEntity->entity=x->dest;
            newCachedEntity->next=NULL;
            first=newCachedEntity;
        } else if(x->count==max){
            newCachedEntity->next=malloc(sizeof(CachedEntity));
            newCachedEntity=newCachedEntity->next;
            newCachedEntity->entity=x->dest;
            newCachedEntity->next=NULL;
        }
        x=relationSuccessor(relationType.dest.nil,x);
    }
    CachedEntity* printCache=first;
    CachedEntity* canc;
    while (printCache!=NULL){
        fputs(" \"",stdout);
        fputs(printCache->entity->name,stdout);
        canc=printCache;
        printCache=printCache->next;
        free(canc);
    }
    fputc(' ',stdout);
    printf("%d",max);
    fputc(';',stdout);
}

void printReport(RelationTypeTree relationTypeTree){
    RelationType* actualRelation=relationTypeMinimum(relationTypeTree.nil,relationTypeTree.root);
    while (actualRelation!=relationTypeTree.nil){
        printRelType(*actualRelation);
        actualRelation=relationTypeSuccessor(&relationTypeTree,actualRelation);
        fputc(' ',stdout);
    }
    fputc('\n',stdout);
}