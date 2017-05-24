/*Header-MicMac-eLiSe-25/06/2007

    MicMac : Multi Image Correspondances par Methodes Automatiques de Correlation
    eLiSe  : ELements of an Image Software Environnement

    www.micmac.ign.fr


    Copyright : Institut Geographique National
    Author : Marc Pierrot Deseilligny
    Contributors : Gregoire Maillet, Didier Boldo.

[1] M. Pierrot-Deseilligny, N. Paparoditis.
    "A multiresolution and optimization-based image matching approach:
    An application to surface reconstruction from SPOT5-HRS stereo imagery."
    In IAPRS vol XXXVI-1/W41 in ISPRS Workshop On Topographic Mapping From Space
    (With Special Emphasis on Small Satellites), Ankara, Turquie, 02-2006.

[2] M. Pierrot-Deseilligny, "MicMac, un lociel de mise en correspondance
    d'images, adapte au contexte geograhique" to appears in
    Bulletin d'information de l'Institut Geographique National, 2007.

Francais :

   MicMac est un logiciel de mise en correspondance d'image adapte
   au contexte de recherche en information geographique. Il s'appuie sur
   la bibliotheque de manipulation d'image eLiSe. Il est distibue sous la
   licences Cecill-B.  Voir en bas de fichier et  http://www.cecill.info.


English :

    MicMac is an open source software specialized in image matching
    for research in geographic information. MicMac is built on the
    eLiSe image library. MicMac is governed by the  "Cecill-B licence".
    See below and http://www.cecill.info.

Header-MicMac-eLiSe-25/06/2007*/
#ifndef __ORI_IMAGE__
#define __ORI_IMAGE__

#include "StdAfx.h"


/* Classe cOri_image: elle g�re un fichier d'orientation d'une image
 * afin de permettre de passer au rep�re terrain et vice versa
 * */ 
 
class cOri_image
{
     public :

        // Constructeur a partir d'un fichier XML , peut etre une calibration interne ou externe
        cOri_image(const std::string &);

        // Sauvegarde dans un fichier
        void Save(const std::string &) const;

        // Fonction de projection  Terrain  -> Image
        Pt2dr Ter2Image(const Pt3dr &) const;

        // Relevement dans l'espace
        std::vector<cOri_image> RelvtEspace
                                (
                                    const Pt3dr & aPTer1, const Pt2dr & aPIm1,
                                    const Pt3dr & aPTer2, const Pt2dr & aPIm2,
                                    const Pt3dr & aPTer3, const Pt2dr & aPIm3
                                );

        // Focale et coeff en R3

            double  Focale () const;
            double  R3 () const;
            Pt2dr   SzCam() const ;

            cOri_image NewCam(double aFoc , double aR3);


        // Manipulation géométrique

           Pt2dr Ter2Im(const Pt3dr & aPTer) const;
           Pt3dr ImAndProf2Ter(const Pt2dr & aPTer,double aProf) const;
           double ProfMoy() const;
           double StepProfOnePixel(const cOri_image &) const;

     private :

        std::string                mName;
        cCalibrationInternConique  mCIO;
        CamStenope *               mCS;
};



/*
 * Classe cImage elle d�finit un objet de tyoe image */
 
class cImage
{
     public :
        cImage (int anX,int anY);
        static cImage  FromString(const std::string &);
        cImage  ImageMoy(int aSzW,int aNbIter);
        cImage  ImageReduite(double aFact);



        void Save(const std::string &);
        void SaveRGB(const std::string &,cImage &, cImage &);

        float GetVal(int anX,int anY) const {return mTIm.get(Pt2di(anX,anY));}

        float GetVal(const Pt2di & aP) const {return mTIm.get(aP);}
        float GetVal(const Pt2dr & aP) const {return mTIm.getr(aP,-1e5);}

        bool Ok(int anX,int anY) const  {return mTIm.inside(Pt2di(anX,anY));}
        Pt2di Sz() const ;

        void  SetVal(int anX,int anY,float aVal) {return mTIm.oset(Pt2di(anX,anY),aVal);}

     private :
        Im2D<float,double>   mIm;
        Pt2di                mSz;
        TIm2D<float,double>  mTIm;
};


#endif // 

/*Footer-MicMac-eLiSe-25/06/2007

Ce logiciel est un programme informatique servant �  la mise en
correspondances d'images pour la reconstruction du relief.

Ce logiciel est régi par la licence CeCILL-B soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL-B telle que diffusée par le CEA, le CNRS et l'INRIA
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  �  l'utilisation,  �  la modification et/ou au
développement et �  la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe �
manipuler et qui le réserve donc �  des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités �  charger  et  tester  l'adéquation  du
logiciel �  leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
�  l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

Le fait que vous puissiez accéder �  cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL-B, et que vous en avez accepté les
termes.
Footer-MicMac-eLiSe-25/06/2007*/
