
#include "stdafx.h"
#include "parameters.h"
#include "simulator.h"


using namespace graphic;

cParametersView::cParametersView() : framework::cDockWindow("Parameters")
, m_incT(0)
, m_isOutVoltage(false)
{
}

cParametersView::~cParametersView()
{
}


bool cParametersView::Init(graphic::cRenderer &renderer)
{
	UpdateNTCRegistance();
	UpdateTemperatureChanges();
	UpdateNCTTemperature();
	UpdateExternalTemperature();

	return true;
}


void cParametersView::OnRender(const float deltaSeconds)
{
	ImGui::Checkbox("Update", &g_root.m_isUpdate);
	ImGui::Spacing();

	ImGui::DragFloat("Temperature", &g_root.m_curTemperature, 1, 0, 100);
	ImGui::DragFloat("Target Temperature", &g_root.m_targetTemperature, 1, 0, 100);
	ImGui::DragInt("Out Voltage Peak Ctrl", &g_root.m_outVPeakNum, 1, 1, 60);

	// NTC Registance Calculation
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Text("NTC Registance By Temperature = Ax + B");
	ImGui::Text("x : temperature");
	const bool edit1 = ImGui::DragFloat("A", &g_root.m_ntcRa, 0.01f, -10.f, 100.f);
	const bool edit2 = ImGui::DragFloat("B", &g_root.m_ntcRb, 0.01f, -10.f, 100.f);

	if (edit1 || edit2)
	{
		UpdateNTCRegistance();
		UpdateNCTTemperature();
		UpdateExternalTemperature();
	}

	ImGui::PlotLines("NTC Registance", g_root.m_ntcRegistance.values
		, ARRAYSIZE(g_root.m_ntcRegistance.values)
		, g_root.m_ntcRegistance.idx, "", 0, 30, ImVec2(0, 130));


	// Temperature Calculation
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Text("Temperature Changes By Current = Cxt + Dt");
	ImGui::Text("x : input current, t : seconds, D : Attenuation");
	ImGui::PushID("temperature");
	const bool edit2_1 = ImGui::DragFloat("C", &g_root.m_tempA, 0.01f, 0.f, 10.f);
	const bool edit2_2 = ImGui::DragFloat("D", &g_root.m_tempAttenuation, 0.01f, 0.f, 10.f);
	ImGui::PopID();

	if (edit2_1 || edit2_2)
	{
		UpdateTemperatureChanges();
		UpdateNCTTemperature();
		UpdateExternalTemperature();
	}

	ImGui::PlotLines("Temperature Changes", g_root.m_tempChanges.values
		, ARRAYSIZE(g_root.m_tempChanges.values)
		, g_root.m_tempChanges.idx, "", 0, 60, ImVec2(0, 130));


	// NCT Temperature Calculation
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Text("NCT Temperature By Current = Cxt + Dt");
	ImGui::Text("x : input current, t : seconds");

	ImGui::PlotLines("NCT Temperature", g_root.m_nctTempChanges.values
		, ARRAYSIZE(g_root.m_nctTempChanges.values)
		, g_root.m_nctTempChanges.idx, "", 0, 200, ImVec2(0, 130));


	// External Temperature Calculation
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Text("External Temperature By Current = CFxt + Dt");
	ImGui::Text("x : input current, t : seconds, F : External Influence");

	ImGui::PushID("External temperature");
	const bool edit3_1 = ImGui::DragFloat("F", &g_root.m_tempExtInfluence, 0.01f, 0.f, 10.f);
	ImGui::PopID();

	if (edit3_1)
		UpdateExternalTemperature();

	ImGui::PlotLines("External Temperature", g_root.m_extTempChanges.values
		, ARRAYSIZE(g_root.m_extTempChanges.values)
		, g_root.m_extTempChanges.idx, "", 0, 200, ImVec2(0, 130));

}


void cParametersView::UpdateNTCRegistance()
{
	const float alpha = 100.f / (float)ARRAYSIZE(g_root.m_ntcRegistance.values);

	g_root.m_ntcRegistance.idx = 0;
	for (int i = 0; i < ARRAYSIZE(g_root.m_ntcRegistance.values); ++i)
	{
		const float x = alpha * i; // temperature
		g_root.m_ntcRegistance.AddValue(max(1, g_root.m_ntcRa * x + g_root.m_ntcRb));
	}
}


void cParametersView::UpdateTemperatureChanges()
{
	g_root.m_tempChanges.idx = 0;
	for (int i = 0; i < ARRAYSIZE(g_root.m_tempChanges.values); ++i)
	{
		const float x = (float)i; // current
		g_root.m_tempChanges.AddValue(g_root.m_tempA * x);
	}
}


void cParametersView::UpdateNCTTemperature()
{
	g_root.m_nctTempChanges.idx = 0;
	for (int i = 0; i < ARRAYSIZE(g_root.m_nctTempChanges.values); ++i)
	{
		const float x = (float)i; // current
		const float v = x * g_root.m_ntcRb;
		const float temp = g_root.m_curTemperature + g_root.m_tempA * x;
		const float r = max(1, g_root.m_ntcRa * temp + g_root.m_ntcRb);
		const float ni = v / r;
		const float ntemp = g_root.m_tempA * ni + g_root.m_curTemperature;
		g_root.m_nctTempChanges.AddValue(ntemp);
	}
}


void cParametersView::UpdateExternalTemperature()
{
	g_root.m_extTempChanges.idx = 0;
	for (int i = 0; i < ARRAYSIZE(g_root.m_extTempChanges.values); ++i)
	{
		const float x = (float)i; // current
		const float v = x * g_root.m_ntcRb;
		const float temp = g_root.m_curTemperature + g_root.m_tempA * x;
		const float r = max(1, g_root.m_ntcRa * temp + g_root.m_ntcRb);
		const float ni = v / r;
		const float ntemp = g_root.m_tempA * ni * g_root.m_tempExtInfluence
			+ g_root.m_curTemperature;
		g_root.m_extTempChanges.AddValue(ntemp);
	}
}
