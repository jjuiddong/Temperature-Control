
#include "stdafx.h"
#include "root.h"


cRoot::cRoot()
	: m_outVPeakNum(5)
	, m_curTemperature(20)
	, m_targetTemperature(20)
	, m_isUpdate(true)
	, m_isRiseSignal(false)
{
	ZeroMemory(&m_inVolts, sizeof(m_inVolts));
	ZeroMemory(&m_Currents, sizeof(m_Currents));
	ZeroMemory(&m_inRiseV, sizeof(m_inRiseV));
	ZeroMemory(&m_outVolts, sizeof(m_outVolts));
	ZeroMemory(&m_extTemperature, sizeof(m_extTemperature));
	ZeroMemory(&m_ntcRegistance, sizeof(m_ntcRegistance));
	ZeroMemory(&m_tempChanges, sizeof(m_tempChanges));
	ZeroMemory(&m_nctTempChanges, sizeof(m_nctTempChanges));
	ZeroMemory(&m_extTempChanges, sizeof(m_extTempChanges));

	// 온도에 따른 ntc 내부저항 크기 그래프
	// Ax + B (x = temperature)
	m_ntcRa = -0.2f;
	m_ntcRb = 20.f;

	// 입력 전류에 따른 온도 변화량
	// Axt + Bt (x = input current, B = attenuation)
	m_tempA = 2.5f;
	m_tempAttenuation = 2.5f;
	m_tempExtInfluence = 0.8f;
}

cRoot::~cRoot()
{
}


bool cRoot::Init()
{

	return true;
}
