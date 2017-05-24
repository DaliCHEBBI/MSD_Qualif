#include "StdAfx.h"
#include <fstream>
//Objective: 


/* 
 This program is aimed to qualif a detetector of Interest Points with 
 the method of qualification based on a DSM: Digital surface model 
 Thus, we need to conduct 2D 3D transformations taking into account 
 an Orientation directory whinch contains the orientations of each image
 
  */


//----------------------------------------------------------------------------
// OrientedImage class
class cOri_Image
{
  public:
    cOri_Image
    (
      std::string aOriIn,
      std::string aName,
      cInterfChantierNameManipulateur * aICNM
    );
    std::string getName(){return mName;}
    CamStenope * getCam(){return mCam;}

  protected:
    CamStenope         * mCam;
    std::string        mName;
    std::string mOriFileName;
 };

cOri_Image::cOri_Image
  (   std::string aOriIn,
   std::string aName,
   cInterfChantierNameManipulateur * aICNM):
    mName(aName),mOriFileName(aOriIn+"Orientation-"+mName+".xml")
{
   mCam=CamOrientGenFromFile(mOriFileName,aICNM);
}


//----------------------------------------------------------------------------

int MSD_Q_main( int argc, char ** argv )
{
	std::cout<<"hello world"<<"\n";

	// Call Elise Librairy We will be using because classes that handle 
	// These transformations are already computed 

		std::string Image_Pattern, Oris_Dir, Keypts_dir, MNS,POINTS_OUT;
		std::string Option;
		
	/***********************************************************************
	 Initilialize ElInitArgMain which as i understood captures arguments entered 
	 by the operator 
	/**********************************************************************/
		ElInitArgMain
		(
			argc,argv,
			LArgMain()  << EAMC(Image_Pattern," Images pattern : Dir+ Name", eSAM_IsPatFile)
			            << EAMC(Oris_Dir,"Orientation files",eSAM_IsExistDirOri)
						<< EAMC (Keypts_dir, " File containing detected MSD INTEREST Points")
						<< EAMC (MNS, "Micmac DIGITAL SURFACE MODEL FORMAT: XML FILE",eSAM_IsExistFile)
						<< EAMC (POINTS_OUT, "Output directory to store projetced points"),
			LArgMain()  << EAM(Option,"Option",true,"FOR NOW, DON'T DO ANYTHING")
		);
/**********************************************************************/
	if (MMVisualMode) return EXIT_SUCCESS;
	
	  MakeFileDirCompl(Oris_Dir);
	  std::cout<<"Oris_Dir dir: "<<Oris_Dir<<std::endl;

/**********************************************************************/  
	// Initialize name manipulator & files
	std::string aDirImages,aPatIm;
	SplitDirAndFile(aDirImages,aPatIm,Image_Pattern);
	std::cout<<"Working dir: "<<aDirImages<<std::endl;
	std::cout<<"Images pattern: "<<aPatIm<<std::endl;
	
/**********************************************************************/  	
	// POint cloud handling 
    std::string aDir,aNC;
    SplitDirAndFile(aDir,aNC,MNS);
    ELISE_ASSERT((aDir==aDirImages), " Point Cloud Xml FILE and images are not in the same directory");
/**********************************************************************/  

    cInterfChantierNameManipulateur * aICNM=cInterfChantierNameManipulateur::BasicAlloc(aDirImages);
    const std::vector<std::string> aSetIm = *(aICNM->Get(aPatIm));
  
    // Check 3D point cloud tree: It needs to have a convenient structure
    cResulMSO aRMso =  aICNM->MakeStdOrient(aNC,false);
    cElNuage3DMaille *  aNuage = aRMso.Nuage();
    ElCamera         * aCam    = aRMso.Cam();
    
    
/**********************************************************************/   
    // Checking the coherence of the suggested 3D point cloud file 
    if (aCam)
    {
		std::cout<<" THe suggested is not a MicMac 3D point cloud  xml file \n";
	}
	
	if (aNuage==NULL)
       {
			std::cout  << "For name " << MNS<< "\n";
			ELISE_ASSERT(aNuage!=0,"The suggested 3D point cloud is not a valid Micmac xml file ");
       }
/**********************************************************************/
/* Now we have a valid 3D point cloud xml file with the possibility   
 *  Of orienting cameras acoording to xml orientation files
 * */
 /*********************************************************************/
 
 
 // CREATE A DIRECTORY IN WHICH ALL PROJECTED POINTS IMAGE2TERRAIN ARE STORED
 //mkdir()

  for (unsigned int i=0; i<aSetIm.size(); i++)
  {
	cOri_Image Im(Oris_Dir,aSetIm[i],aICNM);
	std::string aFilePtsIn, aFilePtsOut;
	aFilePtsIn=Keypts_dir+"MSD-"+Im.getName()+".txt";
	
	
	ELISE_fp aFIn(aFilePtsIn.c_str(),ELISE_fp::READ);
	/*if (aFIn==NULL)
	{
	ELISE_ASSERT((aFIn!=0),"THis is not a valid MSD points file for"+Im.getName());
    }*/
    
	aFilePtsOut=POINTS_OUT+"MSD-"+Im.getName()+"Projected.txt";
	//Im.getCam()->Capteur2RayTer
	
	// Create a file under POINTS_OUT to store projected points
    FILE *  aFOut = FopenNN(aFilePtsOut.c_str(),"w","IM2XYZ");
	
	char * aLine;
	std::vector<Pt2dr> aV2Ok;
	//bool HasEmpty = false;
	
	while ((aLine = aFIn.std_fgets()))
	{
		Pt2dr aPIm;
		int aNb = sscanf(aLine,"%lf %lf",&aPIm.x,&aPIm.y);
		ELISE_ASSERT(aNb==2,"Could not read 2 double values");
		bool HasEmpty=false;
		if (aNuage)
			{
				aPIm = aNuage->ImRef2Capteur (aPIm);/* ici il y a un bug sous linux, segmention core dumped*/

				if (aNuage->CaptHasData(aPIm))
					{
						Pt3dr aP  = aNuage->PreciseCapteur2Terrain(aPIm);
						fprintf(aFOut,"%lf %lf %f\n",aP.x,aP.y,aP.z);
						aV2Ok.push_back(aPIm);
					}
				else
					{
						HasEmpty = true;
						std::cout << "Warn :: " << aPIm << " has no data in cloud\n";
					}
			}
		
	/*	if(Im.getCam()->HasPreciseCapteur2Terrain())
		{
			//aPIm = aNuage->ImRef2Capteur (aPIm);
			std::cout << aPIm << "\n";
			aP=Im.getCam()->PreciseCapteur2Terrain(aPIm);
			fprintf(aFOut,"%lf %lf %lf\n",aP.x,aP.y,aP.z);
	    }
	    else
	    {
			fprintf(aFOut,"Point %lf %lf HAS NO DATA  IN CLOUD", aPIm.x,aPIm.y); 
	    }	*/		
	}
	aFIn.close();
	ElFclose(aFOut);
	}

return EXIT_SUCCESS ;
}
