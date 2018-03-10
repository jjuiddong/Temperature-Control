
#include "stdafx.h"
#include "output.h"
#include "simulator.h"


using namespace graphic;

cOutputView::cOutputView() : framework::cDockWindow("Output")
	, m_riseCnt(0)
	, m_calcCurrent(0)
	, m_incCurrent(0)
	, m_incT(0)
	, m_calcCnt(0)
{
}

cOutputView::~cOutputView()
{
}


bool cOutputView::Init(graphic::cRenderer &renderer)
{
	return true;
}


void cOutputView::OnRender(const float deltaSeconds)
{
	m_incT += deltaSeconds;
	const bool isUpdate = (m_incT >= (1.f / (float)cRoot::FPS));

	if (g_root.m_isRiseSignal)
		m_riseCnt++;

	const int SAMPLING_CNT = 3;
	int peakCnt = (cRoot::HZ * SAMPLING_CNT) / g_root.m_outVPeakNum;
	if (((cRoot::HZ * SAMPLING_CNT) % g_root.m_outVPeakNum) > 0)
		++peakCnt;
	peakCnt = peakCnt * g_root.m_outVPeakNum;

	// sampling 3 seconds
	if (m_riseCnt < peakCnt)
	{
		if (isUpdate)
		{
			m_calcCnt++;
			m_incCurrent += g_root.m_Currents.GetCurValue();
		}
	}
	else
	{
		if (m_calcCnt > 0)
		{
			m_totalCurrent = m_incCurrent;
			m_calcCurrent = m_incCurrent / (float)m_calcCnt;
		}

		m_incCurrent = 0;
		m_riseCnt = 0;
		m_calcCnt = 0;
	}

	ImGui::Text("Rise Signal Count : %d", m_riseCnt);
	ImGui::Text("IncCurrent : %f", m_incCurrent);
	ImGui::Text("Current : %f", m_calcCurrent);
	ImGui::Text("Total Current : %f", m_totalCurrent);

	{
		// i = v / r
		const float curTemp = g_root.m_extTemperature.GetCurValue();
		const float r = (float)cRoot::NTC_R;
		//const float r = max(1, g_root.m_ntcRa * curTemp + g_root.m_ntcRb);
		const float i = (float)cRoot::VOLTAGE / r;

		// http://j1w2k3.tistory.com/907
		// https://namu.wiki/w/%EC%82%AC%EC%9D%B8%EA%B3%A1%EC%84%A0
		// Theoretical Total Current (a*sin(bx+c) integral) = [-1/b * a*cos(b*x + c)] 0 ~ pi/b
		// or, a * sin(bx+c) = 2a / b
		// 
		// a = current
		// b = 2pi * hz
		// x = 1 / (hz * 2)
		const float mountCnt = (float)(peakCnt / g_root.m_outVPeakNum);
		const float t = 1.f / (2.f * cRoot::HZ);
		const float w = 2.f * MATH_PI * cRoot::HZ;
		const float v0 = (-1.f / w) * cos(0.f);
		const float v1 = (-1.f / w) * cos(w * t);
		const float theoreticalCurrent1 = (v1 - v0) * 2 * mountCnt * i;
		const float theoreticalCurrent2 = ((2.f * i) / w) * 2 * mountCnt;

		ImGui::Text("Theoretical Total Current (integral) : %f", theoreticalCurrent1 / (float)SAMPLING_CNT);
	}


	// Control Output Current By cRoot::m_outVPeakNum
	

	if (isUpdate)
	{
		m_incT = 0;
	}
}
