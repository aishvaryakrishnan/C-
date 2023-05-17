/*
 In a card game, each player will be given a set of random cards. Players will throw on the table their one winning card, the player with the highest card wins.
 A winning card is the card that only exists once in the set of cards, and the highest one.
 Given an array of sets of integers cards, return the card of the winning player. Return -1 If no such card is found.
*/

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int winning_card(vector<vector<int>> &cards)
{
  int res = -1;
  for (auto &player_cards : cards)
  {
    std::sort(player_cards.begin(), player_cards.end(), [](int a, int b) { return a > b; } );

    for (auto &card : player_cards)
    {
      if (card > res)
      {
	res = card;
      }
      else if (res == card)
      {
	res = -1;
      }
    }
  }
  
  return res;
}

int main()
{
  std::cout << "enter all the players' cards (space-separated for each player and comma-separated for different players. Example: \"1 2, 5 5\" )" << std::endl;

  vector<vector<int>> all_player_cards;

  std::string input;
  std::getline(std::cin, input);

  int num = -1;
  vector<int> player_cards;
  for(auto &ch : input)
    {
      if (ch == ' ')
      {
	if (num > 0)
	{
	  player_cards.push_back(num);
	  num = -1;
	}
      }
      else if(ch == ',')
      {
	if (num > 0)
	{
	  player_cards.push_back(num);
	  all_player_cards.push_back(player_cards);
	  player_cards.clear();
	  num = -1;
	}
      }
      else
      {
	if (num < 0)
	{
	  num = int(ch) - '0';
	}
	else
	{
	  num = (num * 10) + (int(ch) - '0');
	}
      }
	 
    }

  if (num > 0)
  {
    player_cards.push_back(num);
    all_player_cards.push_back(player_cards);
  }
  std::cout << "winning card is: " << winning_card(all_player_cards) << endl;
}
