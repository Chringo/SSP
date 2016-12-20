#ifndef SSPAPPLICATION_CORE_PROGRESSION_H
#define SSPAPPLICATION_CORE_PROGRESSION_H

class Progression
{

private:

	static Progression* m_instance;

	Progression();


public:
	~Progression();

	static Progression& instance()
	{
		if (m_instance == nullptr) m_instance = new Progression();
		return *m_instance;
	}
};

#endif
