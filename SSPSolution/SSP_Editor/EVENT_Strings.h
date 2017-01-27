#pragma once
#include <qstring.h>
#include "Header.h"
struct EventStrings {
	std::vector<QString> m_typeEvents[NUM_PUZZLE_ELEMENTS];
	std::unordered_map<std::string,int> enumIDs; //Use an int to get a string
	std::unordered_map<int,std::string> enumStrings; // use a string to get an int;
	EventStrings()
	{
		enumIDs.reserve(39);
		m_typeEvents[ContainerType::BUTTON].reserve(5);
		m_typeEvents[ContainerType::BUTTON].push_back("BUTTON_DEACTIVE")  ;
		m_typeEvents[ContainerType::BUTTON].push_back("BUTTON_ACTIVE")	  ;
		m_typeEvents[ContainerType::BUTTON].push_back("BUTTON_RESET")	  ;
		m_typeEvents[ContainerType::BUTTON].push_back("BUTTON_ENABLED")	  ;
		m_typeEvents[ContainerType::BUTTON].push_back("BUTTON_DISABLED")  ;

		enumIDs[m_typeEvents[ContainerType::BUTTON].at(0).toStdString()] =   BUTTON_DEACTIVE ;
		enumIDs[m_typeEvents[ContainerType::BUTTON].at(1).toStdString()] =   BUTTON_ACTIVE	 ;					   
		enumIDs[m_typeEvents[ContainerType::BUTTON].at(2).toStdString()] =   BUTTON_RESET	 ;					   
		enumIDs[m_typeEvents[ContainerType::BUTTON].at(3).toStdString()] =   BUTTON_ENABLED	 ;					   
		enumIDs[m_typeEvents[ContainerType::BUTTON].at(4).toStdString()] =   BUTTON_DISABLED ;	

		enumStrings[ BUTTON_DEACTIVE] = m_typeEvents[ContainerType::BUTTON].at(0).toStdString();
		enumStrings[ BUTTON_ACTIVE	] = m_typeEvents[ContainerType::BUTTON].at(1).toStdString();
		enumStrings[ BUTTON_RESET	] = m_typeEvents[ContainerType::BUTTON].at(2).toStdString();
		enumStrings[ BUTTON_ENABLED	] = m_typeEvents[ContainerType::BUTTON].at(3).toStdString();
		enumStrings[ BUTTON_DISABLED] = m_typeEvents[ContainerType::BUTTON].at(4).toStdString();
																						

		m_typeEvents[ContainerType::DOOR].reserve(5);
		m_typeEvents[ContainerType::DOOR].push_back("DOOR_CLOSED"  );
		m_typeEvents[ContainerType::DOOR].push_back("DOOR_OPENED"	);
		m_typeEvents[ContainerType::DOOR].push_back("DOOR_REMOVED"	);
		m_typeEvents[ContainerType::DOOR].push_back("DOOR_ENABLED"	);
		m_typeEvents[ContainerType::DOOR].push_back("DOOR_DISABLED");

		enumIDs[m_typeEvents[ContainerType::DOOR].at(0).toStdString()] = DOOR_CLOSED  	;
		enumIDs[m_typeEvents[ContainerType::DOOR].at(1).toStdString()] = DOOR_OPENED	;
		enumIDs[m_typeEvents[ContainerType::DOOR].at(2).toStdString()] = DOOR_REMOVED	;
		enumIDs[m_typeEvents[ContainerType::DOOR].at(3).toStdString()] = DOOR_ENABLED	;
		enumIDs[m_typeEvents[ContainerType::DOOR].at(4).toStdString()] = DOOR_DISABLED	;

		enumStrings[ DOOR_CLOSED	] = m_typeEvents[ContainerType::DOOR].at(0).toStdString();
		enumStrings[ DOOR_OPENED	] = m_typeEvents[ContainerType::DOOR].at(1).toStdString();
		enumStrings[ DOOR_REMOVED	] = m_typeEvents[ContainerType::DOOR].at(2).toStdString();
		enumStrings[ DOOR_ENABLED	] = m_typeEvents[ContainerType::DOOR].at(3).toStdString();
		enumStrings[ DOOR_DISABLED	] = m_typeEvents[ContainerType::DOOR].at(4).toStdString();






		m_typeEvents[ContainerType::LEVER].reserve(4);
		m_typeEvents[ContainerType::LEVER].push_back("LEVER_DEACTIVE");
		m_typeEvents[ContainerType::LEVER].push_back("LEVER_ACTIVE");
		m_typeEvents[ContainerType::LEVER].push_back("LEVER_ENABLED" ) ;
		m_typeEvents[ContainerType::LEVER].push_back("LEVER_DISABLED");

		enumIDs[m_typeEvents[ContainerType::LEVER].at(0).toStdString()] = LEVER_DEACTIVE;
		enumIDs[m_typeEvents[ContainerType::LEVER].at(1).toStdString()] = LEVER_ACTIVE  ;
		enumIDs[m_typeEvents[ContainerType::LEVER].at(2).toStdString()] = LEVER_ENABLED ;
		enumIDs[m_typeEvents[ContainerType::LEVER].at(3).toStdString()] = LEVER_DISABLED;
		
		enumStrings[ LEVER_DEACTIVE	]= m_typeEvents[ContainerType::LEVER].at(0).toStdString();
		enumStrings[ LEVER_ACTIVE	]= m_typeEvents[ContainerType::LEVER].at(1).toStdString();
		enumStrings[ LEVER_ENABLED	]= m_typeEvents[ContainerType::LEVER].at(2).toStdString();
		enumStrings[ LEVER_DISABLED	]= m_typeEvents[ContainerType::LEVER].at(3).toStdString();


		m_typeEvents[ContainerType::WHEEL].reserve(18);
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_DECREASING");	
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_INCREASING");	
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_MIN") ;
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_0"	);	
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_10"	 );
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_20"	 );
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_30"	 );
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_40"	 );
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_50"	 );
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_60"	 );
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_70"	 );
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_80"	 );
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_90"	 );
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_100"  ) ;
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_MAX"  ) ;
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_RESET") ;
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_ENABLED") ;
		m_typeEvents[ContainerType::WHEEL].push_back("WHEEL_DISABLED");


		enumIDs[m_typeEvents[ContainerType::WHEEL].at(0).toStdString()]  = WHEEL_DECREASING	;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(1).toStdString()] = WHEEL_INCREASING	;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(2).toStdString()] = WHEEL_MIN		   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(3).toStdString()] = WHEEL_0		   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(4).toStdString()] = WHEEL_10	 	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(5).toStdString()] = WHEEL_20	 	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(6).toStdString()] = WHEEL_30	 	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(7).toStdString()] = WHEEL_40	 	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(8).toStdString()] = WHEEL_50	 	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(9).toStdString()] = WHEEL_60	 	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(10).toStdString()] = WHEEL_70	 	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(11).toStdString()] = WHEEL_80	 	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(12).toStdString()] = WHEEL_90	 	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(13).toStdString()] = WHEEL_100  	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(14).toStdString()] = WHEEL_MAX 	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(15).toStdString()] = WHEEL_RESET 	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(16).toStdString()] = WHEEL_ENABLED	   ;
		enumIDs[m_typeEvents[ContainerType::WHEEL].at(17).toStdString()] = WHEEL_DISABLED;



		enumStrings[ WHEEL_DECREASING	]= m_typeEvents[ContainerType::WHEEL].at(0).toStdString() ;
		enumStrings[ WHEEL_INCREASING	]= m_typeEvents[ContainerType::WHEEL].at(1).toStdString() ;
		enumStrings[ WHEEL_MIN			]= m_typeEvents[ContainerType::WHEEL].at(2).toStdString() ;
		enumStrings[ WHEEL_0			]= m_typeEvents[ContainerType::WHEEL].at(3).toStdString() ;
		enumStrings[ WHEEL_10			]= m_typeEvents[ContainerType::WHEEL].at(4).toStdString() ;
		enumStrings[ WHEEL_20			]= m_typeEvents[ContainerType::WHEEL].at(5).toStdString() ;
		enumStrings[ WHEEL_30			]= m_typeEvents[ContainerType::WHEEL].at(6).toStdString() ;
		enumStrings[ WHEEL_40			]= m_typeEvents[ContainerType::WHEEL].at(7).toStdString() ;
		enumStrings[ WHEEL_50			]= m_typeEvents[ContainerType::WHEEL].at(8).toStdString() ;
		enumStrings[ WHEEL_60			]= m_typeEvents[ContainerType::WHEEL].at(9).toStdString() ;
		enumStrings[ WHEEL_70			]= m_typeEvents[ContainerType::WHEEL].at(10).toStdString();
		enumStrings[ WHEEL_80			]= m_typeEvents[ContainerType::WHEEL].at(11).toStdString();
		enumStrings[ WHEEL_90			]= m_typeEvents[ContainerType::WHEEL].at(12).toStdString();
		enumStrings[ WHEEL_100			]= m_typeEvents[ContainerType::WHEEL].at(13).toStdString();
		enumStrings[ WHEEL_MAX			]= m_typeEvents[ContainerType::WHEEL].at(14).toStdString();
		enumStrings[ WHEEL_RESET		]= m_typeEvents[ContainerType::WHEEL].at(15).toStdString();
		enumStrings[ WHEEL_ENABLED		]= m_typeEvents[ContainerType::WHEEL].at(16).toStdString();
		enumStrings[ WHEEL_DISABLED		]= m_typeEvents[ContainerType::WHEEL].at(17).toStdString();

	}

	int GetEnumIdFromString(QString string)
	{
		std::unordered_map<std::string, int>::iterator got = enumIDs.find(string.toStdString());
		if (got == enumIDs.end()) { // if the model does not exists in memory

			return -1;
		}
		else {
			return got->second;
		}

	}
	QString GetStringFromEnumID(int id)
	{

		
		std::unordered_map< int, std::string>::iterator got = enumStrings.find(id);
		if (got == enumStrings.end()) { // if the model does not exists in memory

			return -1;
		}
		else {
			return QString::fromStdString(got->second);
		}
		
	}

	std::vector<QString>* GetEventStringsFromType(ContainerType type) {

		return &this->m_typeEvents[type];

	}
	
};
