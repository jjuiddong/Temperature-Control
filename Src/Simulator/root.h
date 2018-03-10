//
// Global Control Class
//
#pragma once


template<unsigned int MAXSIZE>
struct sGraph
{
	int idx;
	float values[MAXSIZE];

	void AddValue(const float v) {
		values[idx] = v;
		idx = (idx + 1) % ARRAYSIZE(values);
	}

	float GetCurValue() {
		const int i = (ARRAYSIZE(values) + (idx - 1)) % ARRAYSIZE(values);
		return values[i];
	}

	float GetPrevValue(const int offset = -2) {
		const int i = (ARRAYSIZE(values) + (idx + offset)) % ARRAYSIZE(values);
		return values[i];
	}
};




class cRoot
{
public:
	cRoot();
	virtual ~cRoot();

	bool Init();


public:
	enum {
		FPS = 1000
		, VOLTAGE = 220
		, HZ = 6
		, NTC_R = 20
		, TIME_LINE = 1
	};

	bool m_isUpdate;
	float m_curTemperature; // current temperature
	float m_targetTemperature;
	int m_outVPeakNum;
	bool m_isRiseSignal; // update every frame

	sGraph<FPS * TIME_LINE + 1> m_inVolts;
	sGraph<FPS * TIME_LINE + 1> m_Currents;
	sGraph<FPS * TIME_LINE + 1> m_inRiseV;
	sGraph<FPS * TIME_LINE + 1> m_outVolts;
	sGraph<FPS * TIME_LINE + 1> m_extTemperature;

	// ntc registance
	float m_ntcRa; // ax + b (x = temperature)
	float m_ntcRb;
	sGraph<100> m_ntcRegistance;

	// temperature
	float m_tempA;
	float m_tempAttenuation;
	float m_tempExtInfluence; // external influence constant
	sGraph<20> m_tempChanges;
	sGraph<20> m_nctTempChanges;
	sGraph<20> m_extTempChanges;
};
