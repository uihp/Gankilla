#include <stdlib.h>
#include "main.h"
#include "tetris.h"
#include "../hal/base.h"

int score;
int8_t screen[HEIGHT];
uint8_t block[BLOCK_HEIGHT];
int8_t dropdis, movedis, blktype, blkdire;

bool newb()
{
  int8_t i;
  blktype = rand() % BLOCK_KINDNUM; // Random
  dropdis = 0;
  movedis = 0;
  blkdire = 0;
  for (i = 0; i < BLOCK_HEIGHT; i++)
  {
    block[i] = blocks[blktype][blkdire][i];
    if (screen[i] & block[i]) // Occupied
      return 0;
  }
  return 1;
}

bool drop()
{
  int8_t i;
  for (i = 0; i < BLOCK_HEIGHT; i++)
    if ((dropdis + i + 1 >= HEIGHT && block[i]) || screen[dropdis + i + 1] & block[i]) // Bottomed || Touched
      return 0;
  dropdis++;
  return 1;
}

void fix()
{
  int8_t i;
  for (i = 0; i < BLOCK_HEIGHT; i++)
    screen[dropdis + i] |= block[i];
}

void del()
{
  int8_t i, j;
  for (i = 0; i < HEIGHT; i++)
    if (screen[i] == -1)
    {
      for (j = i; j >= 0; j--)
        screen[j] = j ? screen[j - 1] : 0;
      score++;
    }
}

void move(bool dire)
{
  int8_t i;
  for (i = 0; i < BLOCK_HEIGHT; ++i)
    if (
        (block[i] & 0x80 && !dire) ||
        (block[i] & 0x01 && dire) ||
        screen[dropdis + i] & (dire ? block[i] >> 1 : block[i] << 1)) // Blocked
      return;
  for (i = 0; i < BLOCK_HEIGHT; ++i)
    if (dire)
      block[i] >>= 1;
    else
      block[i] <<= 1;
  movedis += dire ? 1 : -1;
}

#define OBKL blocks[blktype][blkdire][i]
#define MBKL (movedis > 0 ? OBKL >> movedis : OBKL << -movedis)
void change()
{
  int8_t i;
  blkdire++;
  blkdire %= 4;
  for (i = 0; i < BLOCK_HEIGHT; i++)
  {
    if (dropdis + i >= HEIGHT && OBKL) // Bottomed
      return;
    movedis--;
    if (MBKL & 0x01) // Overflow
    {
      movedis++;
      blkdire = blkdire ? blkdire - 1 : 3;
      return;
    }
    movedis++;
    if (screen[dropdis + i] & MBKL) // Blocked
      return;
  }
  for (i = 0; i < BLOCK_HEIGHT; i++)
    block[i] = MBKL;
}

bool playing = 1;
bool falling = 0;
bool speedup = 0;
bool paused = 0;
bool hibernated = 0;

void TRoutine4Tetris()
{
  static int Count4Update = 0, Count4Btn = 0;
  Count4Update++;
  Count4Btn++;
  if (!paused && Count4Update >= 150 - speedup*50)
  {
    Count4Update = 0;
    if (falling)
    {
      falling = drop();
      if (!falling)
        fix();
    }
    else
    {
      del();
      falling = newb();
      playing = falling;
    }
  }
  HAL::keyScan();
  if (*HAL::getKeyFlag() == key::KEY_PRESSED) {
    *HAL::getKeyFlag() = key::KEY_NOT_PRESSED;
    for (unsigned char i = 0; i < key::KEY_NUM; i++) {
      if (HAL::getKeyMap()[i] == key::CLICK) {
        if (i == 2) move(0);
        else if (i == 0) move(1);
        else if (i == 3) change();
        else if (i == 1) while (drop()) {};
      } else if (HAL::getKeyMap()[i] == key::PRESS) {
        if (i == 0) hibernated = 1;
        else if (i == 1) speedup = !speedup;
        else if (i == 2) paused = !paused;
      }
    }
    std::fill(HAL::getKeyMap(), HAL::getKeyMap() + key::KEY_NUM, key::INVALID);
    *HAL::getKeyFlag() = key::KEY_NOT_PRESSED;
  }
}

void showField(unsigned char *dat)
{
  int8_t i, j;
  for (i = 0; i < HEIGHT; i++)
  {
    char row = *(dat + i);
    for (j = 0; j < 8; j++)
      if (row & 0x01 << j)
	  HAL::drawBox(i*8+1, j*8+1, 6, 6);
  }
  HAL::drawFrame(0, 0, 128, 64);
}

int main4Tetris()
{
  int8_t i;
  uint8_t blockLine, flush[HEIGHT];
  srand(HAL::getRandomSeed());
  HAL_TIM_Base_Start_IT(&htim2);
  if (!hibernated) {
    std::fill(screen, screen+HEIGHT, 0);
    std::fill(block, block+BLOCK_HEIGHT, 0);
    score = 0;
    playing = 1;
    falling = 0;
    speedup = 0;
    paused = 0;
  }
  hibernated = 0;
  while (playing && !hibernated)
  {
    for (i = 0; i < HEIGHT; i++)
    {
      blockLine = (dropdis + BLOCK_HEIGHT > i && i >= dropdis ? block[i - dropdis] : 0);
      flush[i] = screen[i] | blockLine;
    }
    HAL::canvasClear();
    showField(flush);
    HAL::canvasUpdate();
  }
  HAL_TIM_Base_Stop_IT(&htim2);
  return score;
}
