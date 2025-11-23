#include "GUIMyFrame1.h"
#include <exiv2/exiv2.hpp>
#include <wx/generic/filedlgg.h>
#include <wx/msgdlg.h>

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{

}

void GUIMyFrame1::m_open_imageOnMenuSelection( wxCommandEvent& event )
{
    wxFileDialog dlg(
        this,
        "Wybierz zdjêcie",
        "",
        "",
        "Obrazy (*.jpg;*.jpeg)|*.jpg;*.jpeg|Wszystkie pliki (*.*)|*.*",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );

    if (dlg.ShowModal() != wxID_OK)
        return;

    wxString path = dlg.GetPath();

    try {
        // Exiv2: wczytanie metadanych
        Exiv2::Image::UniquePtr image =
            Exiv2::ImageFactory::open(path.ToStdString());
        image->readMetadata();

        Exiv2::ExifData& exifData = image->exifData();
        Exiv2::IptcData& iptcData = image->iptcData();

        wxString text;

        text += "Plik: " + path + "\n\n[EXIF]\n";
        for (auto& kv : exifData) {
            text += wxString::FromUTF8(kv.key().c_str());
            text += " = ";
            text += wxString::FromUTF8(kv.toString().c_str());
            text += "\n";
        }

        text += "\n[IPTC]\n";
        for (auto& kv : iptcData) {
            text += wxString::FromUTF8(kv.key().c_str());
            text += " = ";
            text += wxString::FromUTF8(kv.toString().c_str());
            text += "\n";
        }

        // TODO: zamiast MessageBox mo¿esz to wpisaæ do panelu po prawej,
        // jeœli masz np. wxTextCtrl* m_metadataTextCtrl;
        wxMessageBox(text.IsEmpty() ? "Brak metadanych" : text,
            "Metadane EXIF/IPTC",
            wxOK | wxICON_INFORMATION,
            this);
    }
    catch (const Exiv2::Error& e) {
        wxMessageBox(wxString::FromUTF8(e.what()),
            "B³¹d Exiv2",
            wxOK | wxICON_ERROR,
            this);
    }
}

void GUIMyFrame1::m_open_imageOnUpdateUI( wxUpdateUIEvent& event )
{
// TODO: Implement m_open_imageOnUpdateUI
}
