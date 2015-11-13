#include "auxiliarydata.hpp"

namespace OrientationMATIS
{
    const std::string Sample::TestProjWToI = "WorldToImage";
    const std::string Sample::TestUnknown = "Unknown";
    const std::string Sample::TestTransfo2D = "TestTransfo2D";


    bool Date::IsValid() const
    {
        if ((m_jour == 0) || (m_mois == 0)  || (m_annee == 0))
            return false;
        return true;
    }

    void Date::Get(short &annee, unsigned char &mois, unsigned char &jour, unsigned char &heure, unsigned char &minute, float &seconde, std::string &time_system) const
    {
        annee = m_annee;
        mois = m_mois;
        jour = m_jour;
        heure = m_heure;
        minute = m_minute;
        seconde = m_seconde;
        time_system = m_time_system;
    }

    void Date::Set(short annee, unsigned char mois, unsigned char jour, unsigned char heure, unsigned char minute, float seconde, const std::string &time_system)
    {
        m_annee = annee;
        m_mois = mois;
        m_jour = jour;
        m_heure = heure;
        m_minute = minute;
        m_seconde = seconde;
        m_time_system = time_system;
    }



    void AuxiliaryData::AddSample(const std::string &systeme_geodesique, double x, double y, double z, double c, double l)
    {
        Sample s;
        s.systeme_geodesique = systeme_geodesique;
        s.x = x;
        s.y = y;
        s.z = z;
        s.c = c;
        s.l = l;
        AddSample(s);
    }

    void AuxiliaryData::AddSamples(const std::vector <Sample> & VecSample)
    {
        for (unsigned int i = 0 ; i <VecSample.size() ; i++)
            AddSample(VecSample[i]);
    }

    void AuxiliaryData::AddSample(const Sample &s)
    {
        m_samples.push_back(s);
    }

    void AuxiliaryData::SetCameraName(const char *name)
    {
        m_camera_name = name;
    }

    const std::string & AuxiliaryData::GetCameraName() const
    {
        return m_camera_name;
    }

    float AuxiliaryData::GetExposureTime() const
    {
        return m_exposure_time;
    }

    const Date & AuxiliaryData::GetImageDate() const
    {
        return m_image_date;
    }
    const Date & AuxiliaryData::GetCalibrationDate() const
    {
        return m_calibration_date;
    }

    void AuxiliaryData::SetImageDate(const Date &image_date)
    {
        m_image_date = image_date;
    }

    void AuxiliaryData::SetImageDate(short annee, unsigned char mois, unsigned char jour, unsigned char heure, unsigned char minute, float seconde, const std::string &time_system)
    {
        m_image_date.Set(annee, mois, jour, heure, minute, seconde, time_system);
    }

    void AuxiliaryData::SetCalibrationDate(short annee, unsigned char mois, unsigned char jour, unsigned char heure, unsigned char minute, float seconde, const std::string &time_system)
    {
        m_calibration_date.Set(annee, mois, jour, heure, minute, seconde, time_system);
    }

    void AuxiliaryData::SetCalibrationDate(const Date &calibration_date)
    {
        m_calibration_date = calibration_date;
    }
    const Stereopolis & AuxiliaryData::GetStereopolis() const
    {
    	return m_Stereopolis;
    }

}
