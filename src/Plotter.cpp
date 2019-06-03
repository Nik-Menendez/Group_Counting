// Root Imports
#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2Poly.h>
#include <TPad.h>

// MY IMPORTS
#include "Plotter.hpp"


namespace emtf {

Plotter::Plotter(const std::string& _suffix, const std::function<bool (const emtf::Track & track, const float & pt_max) > _filter) :
suffix(_suffix),
filter(_filter),
chi_sqr_1d(("chi_sqr_1d_" + _suffix).c_str(), "Chi Square Dist", 500, 0, 1000),
dr_dz_1d(("dr_dz_1d_" + _suffix).c_str(), "||dr/dz||", 100, 0, 3),
z_for_min_r_1d(("z_for_min_r_1d_" + _suffix).c_str(), "z for min ||r||", 100, -40000, 40000),
min_r_1d(("min_r_1d_" + _suffix).c_str(), "Min ||r||", 500, 0, 1000),
r0_1d(("r0_1d_" + _suffix).c_str(), "||r|| at z=0", 500, 0, 1000),
max_pt_1d(("max_pt_1d_" + _suffix).c_str(), "Max pt", 151, -2, 300),
bx_me4_1d(("bx_me4_1d_" + _suffix).c_str(), "BX Me4", 4, -3.5, 0.5),
r0_2d(("r0_2d_" + _suffix).c_str(), "r at z=0", -500., 500., -500, 500) {
	// r0 2D
	r0_2d.GetXaxis()->SetTitle("X [cm]");
	r0_2d.GetYaxis()->SetTitle("Y [cm]");

	// MAKE HEAT MAP BINS
	const double side = 1000.;
	const double step = 10.;
	const double steps = (side / step);
	double pos_x = -(side / 2.);
	double pos_x2 = pos_x + step;
	double pos_y = -(side / 2.);
	double pos_y2 = pos_y + step;

	for (unsigned int i = 0; i < steps; i++) {
		pos_x = -(side / 2.);
		pos_x2 = pos_x + step;

		for (unsigned int j = 0; j < steps; j++) {
			r0_2d.AddBin(pos_x, pos_y, pos_x2, pos_y2);

			pos_x = pos_x2;
			pos_x2 += step;
		}

		pos_y = pos_y2;
		pos_y2 += step;
	}
}

Plotter::~Plotter() {

}

void Plotter::process(const emtf::Track & track, const float & pt_max) {
	if (!filter(track, pt_max)) {
		return;
	}

	const float chi_sqr = track.getChiSquare();

	const std::array<float, 3> coord_at_z_eq_0 = track.getCoordAtZeq0();
	const std::array<float, 3> direction = track.getDirection();

	const emtf::LinearFitter zx_fitter = track.getZXFitter();
	const emtf::LinearFitter zy_fitter = track.getZYFitter();

	const float a = coord_at_z_eq_0[0];
	const float c = coord_at_z_eq_0[1];

	const float b = direction[0];
	const float d = direction[1];

	const float dr_dz = sqrt(pow(b, 2) + pow(d, 2));

	const float z_for_min_r = -1 * (a * b + c * d) / (pow(b, 2) + pow(d, 2));
	const float x_for_min_r = zx_fitter.predict(z_for_min_r);
	const float y_for_min_r = zy_fitter.predict(z_for_min_r);
	const float min_r = sqrt(pow(x_for_min_r, 2) + pow(y_for_min_r, 2));
	const float r0 = sqrt(pow(a, 2) + pow(c, 2));
	const int me4_hit_bx = track.getHitBX()[3];

	if (!isnan(chi_sqr) && isfinite(chi_sqr)) {
		chi_sqr_1d.Fill(chi_sqr);
	}

	if (!isnan(dr_dz) && isfinite(dr_dz)) {
		dr_dz_1d.Fill(dr_dz);
	}

	if (!isnan(z_for_min_r) && isfinite(z_for_min_r)) {
		z_for_min_r_1d.Fill(z_for_min_r);
	}

	if (!isnan(min_r) && isfinite(min_r)) {
		min_r_1d.Fill(min_r);
	}

	if (!isnan(a) && isfinite(a) && !isnan(c) && isfinite(c)) {
		r0_2d.Fill(a, c, 1);
	}

	if (!isnan(r0) && isfinite(r0)) {
		r0_1d.Fill(r0);
	}

	if (!isnan(pt_max) && isfinite(pt_max)) {
		max_pt_1d.Fill(pt_max);
	}

	if (!isnan(me4_hit_bx) && isfinite(me4_hit_bx)) {
		bx_me4_1d.Fill(me4_hit_bx);
	}
}

void Plotter::write() {
	// INDIVIDUAL
	TCanvas chi_sqr_1d_canvas(("chi_sqr_1d_" + suffix).c_str());
	chi_sqr_1d_canvas.cd(0);
	chi_sqr_1d.Draw();

	TCanvas dr_dz_1d_canvas(("dr_dz_1d_" + suffix).c_str());
	dr_dz_1d_canvas.cd(0);
	dr_dz_1d.Draw();

	TCanvas z_for_min_r_1d_canvas(("z_for_min_r_1d_" + suffix).c_str());
	z_for_min_r_1d_canvas.cd(0);
	z_for_min_r_1d.Draw();

	TCanvas min_r_1d_canvas(("min_r_1d_" + suffix).c_str());
	min_r_1d_canvas.cd(0);
	min_r_1d.Draw();

	TCanvas r0_2d_canvas(("r0_2d_" + suffix).c_str());
	r0_2d_canvas.SetLogz(1);
	r0_2d_canvas.cd(0);
	r0_2d.Draw("COLZ");

	TCanvas r0_1d_canvas(("r0_1d_" + suffix).c_str());
	r0_1d_canvas.SetLogy(1);
	r0_1d_canvas.cd(0);
	r0_1d.Draw();

	TCanvas max_pt_1d_canvas(("max_pt_1d_" + suffix).c_str());
	max_pt_1d_canvas.SetLogy(1);
	max_pt_1d_canvas.cd(0);
	max_pt_1d.Draw();

	TCanvas bx_me4_1d_canvas(("bx_me4_1d_" + suffix).c_str());
	bx_me4_1d_canvas.SetLogy(1);
	bx_me4_1d_canvas.cd(0);
	bx_me4_1d.Draw();

	// COMBINED
	TPad* r0_2d_pad = new TPad(("r0_2d_pad_" + suffix).c_str(), "", 0.005, 0.005, 0.495, 0.995);
	r0_2d_pad->SetLogz(1);
	r0_2d_pad->SetLeftMargin(0.150);
	r0_2d_pad->cd();
	r0_2d.Draw("COLZ");

	TPad* r0_1d_pad = new TPad(("r0_1d_pad_" + suffix).c_str(), "", 0.505, 0.505, 0.995, 0.995);
	r0_1d_pad->SetLogy(1);
	r0_1d_pad->cd();
	r0_1d.Draw();

	TPad* max_pt_1d_pad = new TPad(("max_pt_1d_pad_" + suffix).c_str(), "", 0.505, 0.005, 0.745, 0.495);
	max_pt_1d_pad->SetLogy(1);
	max_pt_1d_pad->cd();
	max_pt_1d.Draw();

	TPad* bx_me4_1d_pad = new TPad(("bx_me4_1d_pad_" + suffix).c_str(), "", 0.755, 0.005, 0.995, 0.495);
	bx_me4_1d_pad->SetLogy(1);
	bx_me4_1d_pad->cd();
	bx_me4_1d.Draw();

	TCanvas r0_canvas(("r0_" + suffix).c_str(), "", 1280, 720);
	r0_canvas.cd();
	r0_2d_pad->Draw();
	r0_1d_pad->Draw();
	max_pt_1d_pad->Draw();
	bx_me4_1d_pad->Draw();

	// WRITE
	chi_sqr_1d_canvas.Write();
	dr_dz_1d_canvas.Write();
	z_for_min_r_1d_canvas.Write();
	min_r_1d_canvas.Write();
	r0_2d_canvas.Write();
	r0_1d_canvas.Write();
	max_pt_1d_canvas.Write();
	bx_me4_1d_canvas.Write();
	r0_canvas.Write();
}
}