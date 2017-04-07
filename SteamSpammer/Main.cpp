#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "steam_api.h"

using namespace std;

LARGE_INTEGER GetPerfFreq()
{
	LARGE_INTEGER Result;
	QueryPerformanceFrequency(&Result);

	return Result;
}

LARGE_INTEGER GetWallClock()
{
	LARGE_INTEGER Result;
	QueryPerformanceCounter(&Result);

	return Result;
}

float GetSecondsElapsed(LARGE_INTEGER StartCounter, LARGE_INTEGER EndCounter)
{
	float Result;
	Result = ((float)EndCounter.QuadPart - (float)StartCounter.QuadPart) / (float)GetPerfFreq().QuadPart;

	return Result;
}

int main()
{
	LARGE_INTEGER Frequency = GetPerfFreq();

	bool IsSteamInitialized = SteamAPI_Init();
	if(IsSteamInitialized)
	{
		while(true)
		{
			system("cls");
			vector<CSteamID> MatchingFriends;

			string SearchString;
			cout << "Search Friends: ";
			getline(cin, SearchString);

			int FriendCount = SteamFriends()->GetFriendCount(k_EFriendFlagImmediate);

			for(int x = 0; x < FriendCount; ++x)
			{
				CSteamID Friend = SteamFriends()->GetFriendByIndex(x, k_EFriendFlagImmediate);
				string Name = SteamFriends()->GetFriendPersonaName(Friend);

				if(Name.find(SearchString) != string::npos)
				{
					MatchingFriends.push_back(Friend);
				}
			}

			cout << endl << endl;
			for(int i = 0; i < MatchingFriends.size(); ++i)
			{
				cout << i << ": " << SteamFriends()->GetFriendPersonaName(MatchingFriends.at(i)) << endl;
			}

			int Choice = 0;
			cout << "Enter number of user: ";
			cin >> Choice;
			cin.get();

			int NumberOfMessages = 0;
			cout << "Enter number of messages to send (same message): ";
			cin >> NumberOfMessages;
			cin.get();

			string Message;
			cout << "Enter Message: ";
			getline(cin, Message);
			
			for(int q = 0; q < NumberOfMessages; ++q)
			{
				LARGE_INTEGER StartCounter = GetWallClock();
				while(GetSecondsElapsed(StartCounter, GetWallClock()) < 0.5f) {}

				string _FormattedMessage;

				_FormattedMessage = Message + " [" + to_string(q + 1) + "]";
				SteamFriends()->ReplyToFriendMessage(MatchingFriends.at(Choice), _FormattedMessage.c_str());
			}
		}
	}
	else
	{
		cout << "Steam Init failed!" << endl;
	}

	SteamAPI_Shutdown();
	system("pause");
	return 0;
}