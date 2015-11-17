#ifndef __AUXILIARYDATA_HPP__
#define __AUXILIARYDATA_HPP__

#include <string>
#include <vector>
#include <iostream>

namespace OrientationMATIS
{
    class Date
    {
        short m_annee;
        unsigned char m_mois, m_jour, m_heure, m_minute;
        float m_seconde;
        std::string m_time_system;
    public:
        Date() { m_seconde = m_annee = m_mois = m_jour = m_heure = m_minute = 0; }
        bool IsValid() const;
       
        void Get(short &annee, unsigned char &mois, unsigned char &jour, unsigned char &heure, unsigned char &minute, float &seconde, std::string &time_system) const;
        void Set(short annee, unsigned char mois, unsigned char jour, unsigned char heure, unsigned char minute, float seconde, const std::string &time_system = "");
    };

    class Sample
    {
    public :
            double x,y,z;
    double c,l;

    std::string systeme_geodesique;
    std::string TestType;

    static const std::string TestTransfo2D;
    static const std::string TestProjWToI;
    static const std::string TestUnknown;
};

   
    //		<stereopolis>
    //		<chantier>Paris_12</chantier>
    //		<date> 80428</date>
    //		<session>807</session>
    //		<section>1</section>
    //		<numero>1</numero>
    //		<position>11</position>
    //		<time_base_ms> 1 </time_base_ms>
    //		<shutter>1740</shutter>
    //		<flatfield_name>ff_test.tif</flatfield_name>
    //		</stereopolis>

    class Stereopolis
    {
    public:
    	bool m_Valide;
    	std::string m_Chantier; 
    	std::string m_Date; 
    	unsigned int m_Session; 
    	unsigned int m_Section;
    	unsigned int m_Numero;
    	unsigned int m_Position; 
    	unsigned int m_Time_base_ms;
    	unsigned int m_Shutter;
    	std::string m_Flatfield_name; 
    
    	Stereopolis () 
    	{
    		m_Valide = false;
    		m_Chantier = m_Date = m_Flatfield_name = "";
    		m_Session = m_Section = m_Numero = m_Position = m_Time_base_ms = m_Shutter = 0;
    	}
    };
      
    
    class AuxiliaryData
    {
    protected:
        std::string m_camera_name;
        Stereopolis m_Stereopolis;
        
        Date m_image_date, m_calibration_date;
        float m_exposure_time;
        
        std::vector<Sample> m_samples;
        //std::string m_chantier;

        std::string m_imageName;
        unsigned int m_SensorSizeX, m_SensorSizeY;

        double m_AltiSol;

        double m_PixelSizeX,m_PixelSizeY;
		


    public:
        AuxiliaryData() : m_camera_name(""), m_Stereopolis(), m_image_date(Date()),
        m_calibration_date(Date()), 
        m_exposure_time(0.) /*, m_chantier("")*/ ,m_imageName(""),
        m_SensorSizeX(0), m_SensorSizeY(0), m_AltiSol(0.),
        m_PixelSizeX(0.), m_PixelSizeY(0.)
        {}

        inline const std::vector<Sample> & GetSamples() const { return m_samples; }
        void AddSample(const std::string &systeme_geodesique, 
        		double x, double y, double z, double c, double l);
        void AddSamples(const std::vector <Sample> & VecSample);
        void AddSample(const Sample &s);

        void SetCameraName(const char *name);
        const std::string & GetCameraName() const;

//        inline void SetChantier(const char *name) { m_chantier = std::string(name); }
//        inline const std::string & GetChantier() const { return m_chantier; }

        inline void SetImageName(const char *name) { m_imageName = std::string(name); }
        inline const std::string & GetImageName() const { return m_imageName; }

        float GetExposureTime() const;

        const Date & GetImageDate() const;
        void SetImageDate(const Date &image_date);
        void SetImageDate(short annee, unsigned char mois, unsigned char jour, unsigned char heure, unsigned char minute, float seconde, const std::string &time_system = "");

        const Date & GetCalibrationDate() const;
        void SetCalibrationDate(short annee, unsigned char mois, unsigned char jour, unsigned char heure, unsigned char minute, float seconde, const std::string &time_system = "");
        void SetCalibrationDate(const Date &calibration_date);
        const Stereopolis & GetStereopolis() const;
        void SetStereopolis(Stereopolis st){m_Stereopolis = st;}
        
        void SetSensorSize(unsigned int x, unsigned int y)
        {
        	m_SensorSizeX = x; 
        	m_SensorSizeY = y;
        }
        
        void GetSensorSize(unsigned int &x, unsigned int &y) const
        {
           	x = m_SensorSizeX ; 
           	y = m_SensorSizeY ;
        }

        void SetAltiSol(double a) {m_AltiSol = a;}
        double GetAltiSol() const {return m_AltiSol;}

        void GetPixelSize(double &a, double &b) const { a =m_PixelSizeX; b = m_PixelSizeY;}
        void SetPixelSize(double a,double b ) {m_PixelSizeX = a; m_PixelSizeY = b;}

    };
} // namespace

#endif //#ifndef __AUXILIARYDATA_HPP__
