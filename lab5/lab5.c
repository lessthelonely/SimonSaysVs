// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "graphics.h"
#include "keyboard.h"
#include "timer.h"
#include "vbe.h"
#include "graph.h"



uint8_t kbc_ih_output, kbc_ih_stat_reg;
bool kbc_ih_err, kbc_ih_obf;

int main(int argc, char *argv[])
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay)
{
  graphics_set_mode(mode);
  sleep(delay);
  vg_exit();
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color)
{

  mode_get_info(mode);
  graphics_set_mode(mode);
  draw_rectangle(x, y, width, height, color);

  int call_back = 0;
  int ipc_status, r;
  uint32_t irq_set;
  message msg;
  uint8_t hook_id;
  int kbd_notification_hook_id;

  call_back |= keyboard_subscribe(&hook_id, &kbd_notification_hook_id);
  irq_set = BIT(hook_id);

  do
  {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status))
    { /* received notification */
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set)
        { /* subscribed interrupt */
          keyboard_handler();
        }
      default:
        continue; /* no other notifications expect do nothing */
      }
    }
    else
    { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  } while (!scancode_is_esc_break(kbc_ih_output)); // Check for exit breakcode since the kbc_ih_output isn't altered in the keyboard_handler and can still be used here

  keyboard_unsubscribe(&kbd_notification_hook_id);
  vg_exit();
  return call_back;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step)
{

  mode_get_info(mode);
  graphics_set_mode(mode);
  uint8_t width, height;
  uint32_t color;
  get_rectangles_size(no_rectangles, &width, &height);
  for (int i = 0; i < no_rectangles; i++)
  {
    for (int j = 0; j < no_rectangles; j++)
    {
      color = get_rectangles_color(mode, first, no_rectangles, step, i, j);
      draw_rectangle(width * j, height * i, width, height, color);
    }
  }

  int call_back = 0;
  int ipc_status, r;
  uint32_t irq_set;
  message msg;
  uint8_t hook_id;
  int kbd_notification_hook_id;

  call_back |= keyboard_subscribe(&hook_id, &kbd_notification_hook_id);
  irq_set = BIT(hook_id);

  do
  {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status))
    { /* received notification */
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set)
        { /* subscribed interrupt */
          keyboard_handler();
        }
      default:
        continue; /* no other notifications expect do nothing */
      }
    }
    else
    { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  } while (!scancode_is_esc_break(kbc_ih_output)); // Check for exit breakcode since the kbc_ih_output isn't altered in the keyboard_handler and can still be used here

  keyboard_unsubscribe(&kbd_notification_hook_id);
  vg_exit();
  return call_back;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y)
{
  
  mode_get_info(0x11A);
  graphics_set_mode(0x11A);
  draw_pix_map(leaderboard_menu_xpm,x,y);
  //draw_pix_map(xpm, x, y);

  int call_back = 0;
  int ipc_status, r;
  uint32_t irq_set;
  message msg;
  uint8_t hook_id;
  int kbd_notification_hook_id;

  call_back |= keyboard_subscribe(&hook_id, &kbd_notification_hook_id);
  irq_set = BIT(hook_id);

  do
  {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status))
    { /* received notification */
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set)
        { /* subscribed interrupt */
          keyboard_handler();
        }
      default:
        continue; /* no other notifications expect do nothing */
      }
    }
    else
    { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  } while (!scancode_is_esc_break(kbc_ih_output)); // Check for exit breakcode since the kbc_ih_output isn't altered in the keyboard_handler and can still be used here

  keyboard_unsubscribe(&kbd_notification_hook_id);
  vg_exit();
  return call_back;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate)
{

  int call_back = 0;
  call_back |= timer_set_frequency(0, fr_rate);
  if (call_back)
  {
    printf("Error setting frequency\n");
    return 1;
  }
  else
  {
    printf("Set frequency\n");
  }

  int sprite_id;
  mode_get_info(0x105);
  graphics_set_mode(0x105);
  printf("Mode set\n");

  create_sprite_memory(1);
  float f_speed;
  if (speed > 0)
  {
    f_speed = speed;
  }
  else
  {
    f_speed = 1 / speed;
  }
  sprite_id = add_sprite(xpm, (float)xi, (float)yi, xf, yf, (bool)(xf - xi) * f_speed, (bool)(yf - yi) * f_speed);
  draw_sprite(sprite_id);

  int ipc_status, r;
  uint32_t kbd_irq_set, timer_irq_set;
  message msg;
  uint8_t kbd_hook_id, timer_hook_id;
  int kbd_notification_hook_id;

  call_back |= keyboard_subscribe(&kbd_hook_id, &kbd_notification_hook_id);
  call_back |= timer_subscribe_int(&timer_hook_id);
  kbd_irq_set = BIT(kbd_hook_id);
  timer_irq_set = BIT(timer_hook_id);

  do
  {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status))
    { /* received notification */
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE: /* hardware interrupt notification */
        printf("Notification\n");
        if (msg.m_notify.interrupts & kbd_irq_set)
        { /* subscribed interrupt */
          printf("Keyboard notification\n");
          keyboard_handler();
        }
        if (msg.m_notify.interrupts & timer_irq_set)
        {
          printf("New frame\n");
          erase_sprite(sprite_id);
          move_sprite(sprite_id);
          draw_sprite(sprite_id);
        }
      default:
        continue; /* no other notifications expect do nothing */
      }
    }
    else
    { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  } while (!scancode_is_esc_break(kbc_ih_output)); // Check for exit breakcode since the kbc_ih_output isn't altered in the keyboard_handler and can still be used here

  keyboard_unsubscribe(&kbd_notification_hook_id);
  timer_unsubscribe_int();
  vg_exit();
  printf("Exiting\n");
  return call_back;
}

int(video_test_controller)()
{
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
