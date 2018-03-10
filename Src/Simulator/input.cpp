
#include "stdafx.h"
#include "input.h"
#include "simulator.h"


using namespace graphic;

cInputView::cInputView() : framework::cDockWindow("Input")
	, m_timeSeconds(0)
	, m_incT(0)
	, m_ctrlPeak(0)
	, m_isOutVoltage(false)
{
}

cInputView::~cInputView()
{
}


bool cInputView::Init(graphic::cRenderer &renderer)
{
	return true;
}


void cInputView::OnRender(const float deltaSeconds)
{
	const float GRAPH_H = 120;

	m_incT += deltaSeconds;
	const bool isUpdate = (m_incT >= (1.f / (float)cRoot::FPS));
	if (isUpdate)
	{
		m_timeSeconds += m_incT;
	}

	// Input Voltage
	if (g_root.m_isUpdate && isUpdate)
	{
		const float wt = MATH_PI * 2.f * cRoot::HZ * m_timeSeconds;
		g_root.m_inVolts.AddValue(sin(wt) * cRoot::VOLTAGE);
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Spacing();

	ImGui::PlotLines("Input Voltage (220V, 60Hz)", g_root.m_inVolts.values
		, ARRAYSIZE(g_root.m_inVolts.values)
		, g_root.m_inVolts.idx, "", -(float)cRoot::VOLTAGE, (float)cRoot::VOLTAGE, ImVec2(0, GRAPH_H));


	// Input Voltage Rise Time
	if (g_root.m_isUpdate && isUpdate)
	{
		const float wt = MATH_PI * 2.f * cRoot::HZ * m_timeSeconds;
		const float v = sin(wt) * (float)cRoot::VOLTAGE;
		const float oldV = g_root.m_inVolts.GetPrevValue();
		const bool riseSignal = ((oldV * v) < 0.f) && (v > 0.f);
		g_root.m_isRiseSignal = riseSignal;
		g_root.m_inRiseV.AddValue((float)riseSignal);
	}
	else
	{
		g_root.m_isRiseSignal = false;
	}

	ImGui::PlotLines("Input Voltage Rise Time(0 ~ 1)", g_root.m_inRiseV.values
		, ARRAYSIZE(g_root.m_inRiseV.values)
		, g_root.m_inRiseV.idx, "", 0, 1, ImVec2(0, GRAPH_H));


	// Control Voltage
	if (g_root.m_isUpdate && isUpdate)
	{
		const float wt = MATH_PI * 2.f * cRoot::HZ * m_timeSeconds;
		const float v = sin(wt) * (float)cRoot::VOLTAGE;
		const float i = abs(v / cRoot::NTC_R);
		const float oldV = g_root.m_inVolts.GetPrevValue();
		const bool peak = ((oldV * v) < 0.f) && (v > 0.f);
		if (peak)
			++m_ctrlPeak;

		if (peak)
		{
			if (m_ctrlPeak >= g_root.m_outVPeakNum)
			{
				m_ctrlPeak = 0;
				m_isOutVoltage = true;
			}
			else
			{
				m_isOutVoltage = false;
			}
		}
		
		g_root.m_outVolts.AddValue(v * (float)m_isOutVoltage);
	}

	ImGui::PlotLines("Control Voltage (-220 ~ +220)", g_root.m_outVolts.values
		, ARRAYSIZE(g_root.m_outVolts.values)
		, g_root.m_outVolts.idx, "", -220, 220, ImVec2(0, GRAPH_H));


	// Input Current
	if (g_root.m_isUpdate && isUpdate)
	{
		const float wt = MATH_PI * 2.f * cRoot::HZ * m_timeSeconds;
		const float v = abs(g_root.m_outVolts.GetCurValue());
		const float i = v / cRoot::NTC_R;

		// apply temperature registance
		const float x = (float)i; // current
		const float temp = g_root.m_curTemperature + g_root.m_tempA * x;
		const float r = max(1, g_root.m_ntcRa * temp + g_root.m_ntcRb);
		const float ni = v / r;

		g_root.m_Currents.AddValue(ni);
	}

	ImGui::PlotLines("Input Current (0 ~ 30)", g_root.m_Currents.values
		, ARRAYSIZE(g_root.m_Currents.values)
		, g_root.m_Currents.idx, "", 0, 30, ImVec2(0, GRAPH_H));


	// Temperature
	if (g_root.m_isUpdate && isUpdate)
	{
		const float i = g_root.m_Currents.GetCurValue();
		const float oldT = g_root.m_extTemperature.GetCurValue();
		const float chT = (i * g_root.m_tempA * g_root.m_tempExtInfluence * m_incT) 
			- (g_root.m_tempAttenuation * m_incT);
		const float temp = max(g_root.m_curTemperature, oldT + chT);

		g_root.m_extTemperature.AddValue(temp);
	}

	ImGui::PlotLines("External Temperature (-10 ~ +100)", g_root.m_extTemperature.values
		, ARRAYSIZE(g_root.m_extTemperature.values)
		, g_root.m_extTemperature.idx, "", -10, 100, ImVec2(0, GRAPH_H));


	if (isUpdate)
	{
		m_incT = 0;
	}
}
