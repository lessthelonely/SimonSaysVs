#include <lcom/lcf.h>
#include "num_and_letters.h"

void loadnums()
{
  xpm_load(Zero_xpm, XPM_5_6_5, &numbers[0]);
  xpm_load(One_xpm, XPM_5_6_5, &numbers[1]);
  xpm_load(Two_xpm, XPM_5_6_5, &numbers[2]);
  xpm_load(Three_xpm, XPM_5_6_5, &numbers[3]);
  xpm_load(Four_xpm, XPM_5_6_5, &numbers[4]);
  xpm_load(Five_xpm, XPM_5_6_5, &numbers[5]);
  xpm_load(Six_xpm, XPM_5_6_5, &numbers[6]);
  xpm_load(Seven_xpm, XPM_5_6_5, &numbers[7]);
  xpm_load(Eight_xpm, XPM_5_6_5, &numbers[8]);
  xpm_load(Nine_xpm, XPM_5_6_5, &numbers[9]);
}

void loadlet()
{
  xpm_load(A_xpm, XPM_5_6_5, &letters[0]);
  xpm_load(B_xpm, XPM_5_6_5, &letters[1]);
  xpm_load(C_xpm, XPM_5_6_5, &letters[2]);
  xpm_load(D_xpm, XPM_5_6_5, &letters[3]);
  xpm_load(E_xpm, XPM_5_6_5, &letters[4]);
  xpm_load(F_xpm, XPM_5_6_5, &letters[5]);
  xpm_load(G_xpm, XPM_5_6_5, &letters[6]);
  xpm_load(H_xpm, XPM_5_6_5, &letters[7]);
  xpm_load(I_xpm, XPM_5_6_5, &letters[8]);
  xpm_load(J_xpm, XPM_5_6_5, &letters[9]);
  xpm_load(K_xpm, XPM_5_6_5, &letters[10]);
  xpm_load(L_xpm, XPM_5_6_5, &letters[11]);
  xpm_load(M_xpm, XPM_5_6_5, &letters[12]);
  xpm_load(N_xpm, XPM_5_6_5, &letters[13]);
  xpm_load(O_xpm, XPM_5_6_5, &letters[14]);
  xpm_load(P_xpm, XPM_5_6_5, &letters[15]);
  xpm_load(Q_xpm, XPM_5_6_5, &letters[16]);
  xpm_load(R_xpm, XPM_5_6_5, &letters[17]);
  xpm_load(S_xpm, XPM_5_6_5, &letters[18]);
  xpm_load(T_xpm, XPM_5_6_5, &letters[19]);
  xpm_load(U_xpm, XPM_5_6_5, &letters[20]);
  xpm_load(V_xpm, XPM_5_6_5, &letters[21]);
  xpm_load(W_xpm, XPM_5_6_5, &letters[22]);
  xpm_load(X_xpm, XPM_5_6_5, &letters[23]);
  xpm_load(Y_xpm, XPM_5_6_5, &letters[24]);
  xpm_load(Z_xpm, XPM_5_6_5, &letters[25]);
  xpm_load(SPACE_xpm, XPM_5_6_5, &letters[26]);
  xpm_load(empty_xpm, XPM_5_6_5, &letters[27]);
}

void loadlilnums()
{
  xpm_load(Zero_lil_xpm, XPM_5_6_5, &lilnumbers[0]);
  xpm_load(One_lil_xpm, XPM_5_6_5, &lilnumbers[1]);
  xpm_load(Two_lil_xpm, XPM_5_6_5, &lilnumbers[2]);
  xpm_load(Three_lil_xpm, XPM_5_6_5, &lilnumbers[3]);
  xpm_load(Four_lil_xpm, XPM_5_6_5, &lilnumbers[4]);
  xpm_load(Five_lil_xpm, XPM_5_6_5, &lilnumbers[5]);
  xpm_load(Six_lil_xpm, XPM_5_6_5, &lilnumbers[6]);
  xpm_load(Seven_lil_xpm, XPM_5_6_5, &lilnumbers[7]);
  xpm_load(Eight_lil_xpm, XPM_5_6_5, &lilnumbers[8]);
  xpm_load(Nine_lil_xpm, XPM_5_6_5, &lilnumbers[9]);
}

void loadlilet()
{
  xpm_load(A_lil_xpm, XPM_5_6_5, &lilletters[0]);
  xpm_load(B_lil_xpm, XPM_5_6_5, &lilletters[1]);
  xpm_load(C_lil_xpm, XPM_5_6_5, &lilletters[2]);
  xpm_load(D_lil_xpm, XPM_5_6_5, &lilletters[3]);
  xpm_load(E_lil_xpm, XPM_5_6_5, &lilletters[4]);
  xpm_load(F_lil_xpm, XPM_5_6_5, &lilletters[5]);
  xpm_load(G_lil_xpm, XPM_5_6_5, &lilletters[6]);
  xpm_load(H_lil_xpm, XPM_5_6_5, &lilletters[7]);
  xpm_load(I_lil_xpm, XPM_5_6_5, &lilletters[8]);
  xpm_load(J_lil_xpm, XPM_5_6_5, &lilletters[9]);
  xpm_load(K_lil_xpm, XPM_5_6_5, &lilletters[10]);
  xpm_load(L_lil_xpm, XPM_5_6_5, &lilletters[11]);
  xpm_load(M_lil_xpm, XPM_5_6_5, &lilletters[12]);
  xpm_load(N_lil_xpm, XPM_5_6_5, &lilletters[13]);
  xpm_load(O_lil_xpm, XPM_5_6_5, &lilletters[14]);
  xpm_load(P_lil_xpm, XPM_5_6_5, &lilletters[15]);
  xpm_load(Q_lil_xpm, XPM_5_6_5, &lilletters[16]);
  xpm_load(R_lil_xpm, XPM_5_6_5, &lilletters[17]);
  xpm_load(S_lil_xpm, XPM_5_6_5, &lilletters[18]);
  xpm_load(T_lil_xpm, XPM_5_6_5, &lilletters[19]);
  xpm_load(U_lil_xpm, XPM_5_6_5, &lilletters[20]);
  xpm_load(V_lil_xpm, XPM_5_6_5, &lilletters[21]);
  xpm_load(W_lil_xpm, XPM_5_6_5, &lilletters[22]);
  xpm_load(X_lil_xpm, XPM_5_6_5, &lilletters[23]);
  xpm_load(Y_lil_xpm, XPM_5_6_5, &lilletters[24]);
  xpm_load(Z_lil_xpm, XPM_5_6_5, &lilletters[25]);
  xpm_load(Bar_xpm, XPM_5_6_5, &lilletters[26]);
}

xpm_image_t *get_prop_from_char(char byte)
{
  if (byte >= 'a' && byte <= 'z')
  {
    return &letters[byte - 'a'];
  }
  else if (byte == ' ')
  {
    return &letters[26];
  }
  else if (byte >= '0' && byte <= '9')
  {

    return &numbers[byte - '0'];
  }
  else if (byte == '/')
  {
    return &letters[28];
  }
  return &letters[26];
}

xpm_image_t *get_lilprop_from_char(char byte)
{
  if (byte == 'i')
  {
    return &lilletters[8];
  }
  else if (byte == '3')
  {
    return &lilnumbers[3];
  }
  else if (byte >= 'a' && byte <= 'z')
  {
    return &lilletters[byte - 'a'];
  }
  else if (byte >= '0' && byte <= '9')
  {

    return &lilnumbers[byte - '0'];
  }
  else if (byte == '/')
  {
    return &lilletters[26];
  }
  return &lilletters[26];
}

bool is_alpha_numberical(char byte)
{
  if (byte >= 'a' && byte <= 'z')
  {
    return true;
  }
  if (is_numerical(byte))
  {
    return true;
  }

  if (byte == ' ')
  {
    return true;
  }
  return false;
}

bool is_numerical(char byte)
{
  return byte >= '0' && byte <= '9';
}

xpm_image_t get_empty()
{
  return letters[27];
}

xpm_image_t *get_mouse()
{
  xpm_load(mouse_xpm, XPM_5_6_5, &mouse_img);

  return &mouse_img;
}
