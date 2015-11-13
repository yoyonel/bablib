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
		Date();
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
	


	class AuxiliaryData
	{
	protected:
		std::string m_camera_name;
		Date m_image_date, m_calibration_date;
		float m_exposure_time;
		std::vector<Sample> m_samples;
	public:
		AuxiliaryData();

		const std::vector<Sample> & GetSamples() const;
		void AddSample(const std::string &systeme_geodesique, double x, double y, double z, double c, double l);
		void AddSamples(const std::vector <Sample> & VecSample);
		void AddSample(const Sample &s);

		void SetCameraName(const char *name);
		const std::string & GetCameraName() const;

		float GetExposureTime() const;

		const Date & GetImageDate() const;
		void SetImageDate(const Date &image_date);
		void SetImageDate(short annee, unsigned char mois, unsigned char jour, unsigned char heure, unsigned char minute, float seconde, const std::string &time_system = "");

		const Date & GetCalibrationDate() const;
		void SetCalibrationDate(short annee, unsigned char mois, unsigned char jour, unsigned char heure, unsigned char minute, float seconde, const std::string &time_system = "");
		void SetCalibrationDate(const Date &calibration_date);
	};
} // namespace

#endif //#ifndef __AUXILIARYDATA_HPP__
