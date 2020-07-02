#include <memory.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <locale.h>
#include <ncurses.h>
// utility functions

void *mallocz(size_t size) {
  void *ptr;
  ptr = malloc(size);
  if (!ptr)
    return NULL;
  memset(ptr, 0, size);
  return ptr;
}

// definitions

// 2660  BLACK SPADE SUIT
// 2661  WHITE HEART SUIT
// 2662  WHITE DIAMOND SUIT
// 2663  BLACK CLUB SUIT
// 2664  WHITE SPADE SUIT
// 2665  BLACK HEART SUIT
// 2666  BLACK DIAMOND SUIT
// 2667  WHITE CLUB SUIT

enum {
  SUIT_HEART,   //
  SUIT_SPADE,   //
  SUIT_CLUB,    //
  SUIT_DIAMOND, //
  SUIT_COUNT    // ♥♠♣♦
};

enum {
  RANK_A,
  RANK_2,
  RANK_3,
  RANK_4,
  RANK_5,
  RANK_6,
  RANK_7,
  RANK_8,
  RANK_9,
  RANK_10,
  RANK_J,
  RANK_Q,
  RANK_K,
  RANK_COUNT
};

typedef struct card {
  int suit;
  int rank;
  int revealed;
} card;

typedef card *card_ptr;

#define UNICODE
#ifdef UNICODE
const char *suit_to_charptr(int suit) {
  switch (suit) {
  case SUIT_HEART:
    return "\u2665";
  case SUIT_SPADE:
    return "\u2660";
  case SUIT_CLUB:
    return "\u2663";
  case SUIT_DIAMOND:
    return "\u2666";
  default:
    return "?";
  }
}

#else
const char *suit_to_charptr(int suit) {
  switch (suit) {
  case SUIT_HEART:
    return "H";
  case SUIT_SPADE:
    return "S";
  case SUIT_CLUB:
    return "C";
  case SUIT_DIAMOND:
    return "D";
  default:
    return "?";
  }
}
#endif

const char *rank_to_charptr(int rank) {
  switch (rank) {
  case RANK_A:
    return "A";
  case RANK_2:
    return "2";
  case RANK_3:
    return "3";
  case RANK_4:
    return "4";
  case RANK_5:
    return "5";
  case RANK_6:
    return "6";
  case RANK_7:
    return "7";
  case RANK_8:
    return "8";
  case RANK_9:
    return "9";
  case RANK_10:
    return "10";
  case RANK_J:
    return "J";
  case RANK_Q:
    return "Q";
  case RANK_K:
    return "K";
  default:
    return "?";
  }
}

void print_card(card *c) {
  printf("%s%s", rank_to_charptr(c->rank), suit_to_charptr(c->suit));
}

void print_card_ptr(card *c) { printf("%p", c); }

card make_card(int suit, int rank) {
  card c;
  c.suit = suit;
  c.rank = rank;
  return c;
}

card *make_card_ptr(int suit, int rank) {
  card *c = mallocz(sizeof(card));
  c->suit = suit;
  c->rank = rank;
  return c;
}

int is_black(card c) { return c.suit == SUIT_CLUB || c.suit == SUIT_SPADE; }

int is_red(card c) { return c.suit == SUIT_HEART || c.suit == SUIT_DIAMOND; }

int is_ace(card c) { return c.rank == RANK_A; }

int is_alternate_color(card first, card second) {
  return is_black(first) != is_black(second);
}

int is_in_sequence(card lower, card higher) {
  return higher.rank == lower.rank + 1;
}

int can_be_placed_bottom(card parent, card child) {
  return is_alternate_color(parent, child) && is_in_sequence(child, parent);
}

int is_same_suit(card first, card second) { return first.suit == second.suit; }

int can_be_placed_on_foundation(card parent, card child) {
  return is_same_suit(parent, child) && is_in_sequence(parent, child);
}

#define CARD_COUNT 52

#define PILE_LIST

#ifdef PILE_LIST

typedef struct card_node {
  card *value;
  struct card_node *next;
} card_node;

typedef struct pile {
  card_node *head;
  int num_cards;
} pile;

card_node *find_tail(pile *pile) {
  card_node *tail = pile->head;
  if (tail == 0)
    return 0;
  while (tail->next != 0) {
    tail = tail->next;
  }
  return tail;
}

card_node *make_node(card *card) {
  card_node *node = mallocz(sizeof(card_node));
  node->value = card;
  node->next = 0;
  return node;
}

int is_empty(pile *pile) { return pile->num_cards == 0; }

// remove a card from a pile, relinking the list
void delete (pile *pile, card *card) {
  if (is_empty(pile)) {
    return;
  }
  // no previous node for the first item
  card_node *prev = NULL;
  card_node *current;
  for (current = pile->head; current != NULL;
       prev = current, current = current->next) {
    if (current->value == card) {
      // special case if the first item was found
      if (prev == NULL) {
        pile->head = NULL;
      } else {
        // skip over the current item
        prev->next = current->next;
      }
      pile->num_cards--;
      free(current);
      return;
    }
  }
}

// append to the end of the list
void push(pile *pile, card *card) {
  card_node *tail = find_tail(pile);
  if (tail == NULL) {
    pile->head = make_node(card);
  } else {
    tail->next = make_node(card);
  }
  pile->num_cards++;
}

// remove a card from the end of the list
card *pop(pile *pile) {
  pile->num_cards--;
  // find the (n-1)th card
  card_node *pre_tail = pile->head;
  for (int i = 0; i < pile->num_cards - 1; i++)
    pre_tail = pre_tail->next;
  card_node *tail = pre_tail->next;
  card *card = tail->value;
  pre_tail->next = 0;
  free(tail);
  return card;
}

// append to the beginning of the list
void unshift(pile *pile, card *card) {
  card_node *new_head = make_node(card);
  new_head->next = pile->head;
  pile->head = new_head;
  pile->num_cards++;
}

// remove a card from the beginning of the list
card *shift(pile *pile) {
  pile->num_cards--;
  card_node *old_head = pile->head;
  pile->head = old_head->next;

  card *card = old_head->value;
  // free(old_head);
  return card;
}

card *peek_card_at(pile *pile, int index) {
  card_node *head = pile->head;
  for (int i = 0; i < index; i++)
    head = head->next;
  return head->value;
}

card *peek(pile *pile) {
  if (pile->head == NULL) {
    return NULL;
  }
  return pile->head->value;
}

card *peek_last(pile *pile) {
  if (pile->head == NULL) {
    return NULL;
  }
  return peek_card_at(pile, pile->num_cards - 1);
}

pile *make_pile() {
  pile *pile_ptr = mallocz(sizeof(pile *));
  pile_ptr->num_cards = 0;
  return pile_ptr;
}

void fill_deck(pile *pile) {
  for (int rank = 0; rank < RANK_COUNT; rank++) {
    for (int suit = 0; suit < SUIT_COUNT; suit++) {
      push(pile, make_card_ptr(suit, rank));
    }
  }
}
#endif

#define COLUMN_COUNT 7
#define FOUNDATION_COUNT 4

enum {
  PILE_DECK,
  PILE_REVEALED,
  PILE_FOUNDATION1,
  PILE_FOUNDATION2,
  PILE_FOUNDATION3,
  PILE_FOUNDATION4,
  PILE_COLUMN1,
  PILE_COLUMN2,
  PILE_COLUMN3,
  PILE_COLUMN4,
  PILE_COLUMN5,
  PILE_COLUMN6,
  PILE_COLUMN7,
  PILE_COUNT
};

typedef struct game_state {
  pile **piles;
  int pile_count;
} game_state;

game_state *make_game_state() {
  game_state *state = mallocz(sizeof(game_state));
  state->piles = mallocz(sizeof(pile *) * PILE_COUNT);
  for (int pile_idx = 0; pile_idx < PILE_COUNT; pile_idx++) {
    state->piles[pile_idx] = make_pile();
  }
  return state;
}

void test_cards() {
  card c5H = make_card(SUIT_HEART, RANK_5);
  card c5S = make_card(SUIT_SPADE, RANK_5);
  card c6H = make_card(SUIT_HEART, RANK_6);
  card c6D = make_card(SUIT_DIAMOND, RANK_6);
  card c5D = make_card(SUIT_DIAMOND, RANK_5);
  card c7D = make_card(SUIT_DIAMOND, RANK_7);
  printf("5s is black %d vs 1 \n", is_black(c5S));
  printf("5s is red %d vs 0 \n", is_red(c5S));
  printf("6h is red %d vs 1 \n", is_red(c6H));
  printf("6h is black %d vs 0 \n", is_black(c6H));
  printf("5h 6h is alternate %d vs 0 \n", is_alternate_color(c5H, c6H));
  printf("5s 6h is alternate %d vs 1 \n", is_alternate_color(c5S, c6H));
  printf("5d 6d is same suit %d vs 1 \n", is_same_suit(c5D, c6D));
  printf("6h 6d is same suit %d vs 0 \n", is_same_suit(c6H, c6D));
  printf("5s 6h in sequence %d vs 1 \n", is_in_sequence(c5S, c6H));
  printf("6h 6d in sequence %d vs 0 \n", is_in_sequence(c6H, c6D));
  printf("5s 7d in sequence %d vs 0 \n", is_in_sequence(c5S, c7D));
  printf("5h 6h can be placed on foundation %d vs 1\n",
         can_be_placed_on_foundation(c5H, c6H));
  printf("5h 6h can be placed on foundation %d vs 0\n",
         can_be_placed_on_foundation(c6H, c5H));
  printf("6h 5s can be placed on the bottom %d vs 1\n",
         can_be_placed_bottom(c6H, c5S));
  printf("5h 6h can be placed on the bottom %d vs 0\n",
         can_be_placed_bottom(c5H, c6H));
  printf("6h 5h can be placed on the bottom %d vs 0\n",
         can_be_placed_bottom(c6H, c5H));
  // TODO can_be_placed_on_bottom test
}

void print_deck(pile *pile) {
  printf("[");
  card_node *head = pile->head;
  for (int i = 0; i < pile->num_cards; i++) {
    card *c = head->value;
    print_card(c);
    printf(" ");
    head = head->next;
  }
  printf("] (%d)\n", pile->num_cards);
}

void print_pile_ptrs(pile *pile) {
  printf("@[");
  card_node *head = pile->head;
  for (int i = 0; i < pile->num_cards; i++) {
    card *c = head->value;
    print_card(c);
    printf(" @ %p | ", c);
    // second row peek
    // for (int i = 0; i < COLUMN_COUNT; i++) {
    //  move(6, column_size * i);
    //  printw_card(peek(column(state, i)));
    //}

    head = head->next;
  }
  printf("] (%d)\n", pile->num_cards);
}

void insert(pile *pile, card *card, int idx) {
  card_node *pre_tail = pile->head;
  for (int i = 0; i < idx; i++)
    pre_tail = pre_tail->next;
  card_node *card_node = make_node(card);
  card_node->next = pre_tail->next;
  pre_tail->next = card_node;
  pile->num_cards++;
}

void shuffle_pile(pile *pile) {
  int shuffle_times = pile->num_cards * 10;
  for (int i = 0; i < shuffle_times; i++) {
    // unshift a card and insert to random place
    int idx = rand() % pile->num_cards - 1;
    card_ptr card = shift(pile);
    insert(pile, card, idx);
  }
}

#define PRINTCARD(x)                                                           \
  print_card(x);                                                               \
  printf(" ");                                                                 \
  print_card_ptr(x);                                                           \
  printf("\n");

void test_pile_operations() {
  pile *initial_deck = make_pile();
  fill_deck(initial_deck);
  print_deck(initial_deck);
  print_pile_ptrs(initial_deck);

  printf("\npop\n");
  card_ptr popped_card = pop(initial_deck);
  PRINTCARD(popped_card);
  print_deck(initial_deck);
  print_pile_ptrs(initial_deck);

  printf("\npop\n");
  card_ptr popped_card_2 = pop(initial_deck);
  PRINTCARD(popped_card_2);
  print_deck(initial_deck);
  print_pile_ptrs(initial_deck);

  printf("\nshift\n");
  card_ptr dequeued_card = shift(initial_deck);
  PRINTCARD(dequeued_card);
  print_deck(initial_deck);
  print_pile_ptrs(initial_deck);

  printf("\nshift\n");
  card_ptr dequeued_card_2 = shift(initial_deck);
  PRINTCARD(dequeued_card_2);
  print_deck(initial_deck);
  print_pile_ptrs(initial_deck);

  printf("\nunshift 2H\n");
  unshift(initial_deck, dequeued_card_2);
  print_deck(initial_deck);
  print_pile_ptrs(initial_deck);

  printf("\nunshift 3H\n");
  unshift(initial_deck, dequeued_card);
  print_deck(initial_deck);
  print_pile_ptrs(initial_deck);

  printf("\npush 4H\n");
  push(initial_deck, popped_card_2);
  print_deck(initial_deck);
  print_pile_ptrs(initial_deck);
}

pile *stock(game_state *state) { return state->piles[PILE_DECK]; }

pile *waste(game_state *state) { return state->piles[PILE_REVEALED]; }

pile *column(game_state *state, int index_one_based) {
  return state->piles[PILE_COLUMN1 + index_one_based - 1];
}

pile *foundation(game_state *state, int index_one_based) {
  return state->piles[PILE_FOUNDATION1 + index_one_based - 1];
}

void reveal(card *card) {
  if (card == NULL)
    return;
  card->revealed = 1;
}

void hide(card *card) {
  if (card == NULL)
    return;
  card->revealed = 0;
}

void turn(game_state *state) {
  // moves 1 card from stock to waste
  card *revealed_card = shift(stock(state));
  reveal(revealed_card);
  push(state->piles[PILE_REVEALED], revealed_card);
}

void deal(game_state *state) {
  // assuming a shuffled deck
  pile *deck = state->piles[PILE_DECK];
  // deal columns
  for (int i = 0; i < COLUMN_COUNT; i++) {
    int column_idx = i + 1;
    pile *column = state->piles[PILE_COLUMN1 + i];
    // deal N cards in Nth column
    for (int card_num = 0; card_num < column_idx; card_num++) {
      card *card = shift(deck);
      push(column, card);
      // reveal last card from the column
      if (card_num == column_idx - 1) {
        reveal(card);
      }
    }
  }
  // reveal 1 card
  turn(state);
}

void print_all(game_state *state) {
  printf("\nstock:\n");
  print_deck(stock(state));
  printf("\nrevealed:\n");
  print_deck(state->piles[PILE_REVEALED]);

  for (int i = 0; i < COLUMN_COUNT; i++) {
    int column = i + 1;
    printf("\ncolumn %d\n", column);
    print_deck(state->piles[PILE_COLUMN1 + i]);
  }
}

int rows, cols;

#define BLACK_PAIR 1
#define RED_PAIR 2
#define DEFAULT_COLOR -1

void init_curses() {
  initscr();
  use_default_colors();
  start_color();
  getmaxyx(stdscr, rows, cols);
  init_pair(BLACK_PAIR, DEFAULT_COLOR, DEFAULT_COLOR);
  init_pair(RED_PAIR, COLOR_RED, DEFAULT_COLOR);
}

void printw_card(card *c) {
  if (c == NULL) {
    printw("<NULL>");
    return;
  }
  int color_pair = is_black(*c) ? BLACK_PAIR : RED_PAIR;
  attron(COLOR_PAIR(color_pair));
  if (c->revealed) {
    printw("%s%s", rank_to_charptr(c->rank), suit_to_charptr(c->suit));
  } else {
    printw("(%s%s)", rank_to_charptr(c->rank), suit_to_charptr(c->suit));
  }
  attroff(COLOR_PAIR(color_pair));
}

void printw_pile_size(pile *pile) { printw("(%d cards)", pile->num_cards); }

void end_curses() { endwin(); }

char *first_row_headers[] = {"Stock",        "Waste",        "",
                             "Foundation 1", "Foundation 2", "Foundation 3",
                             "Foundation 4"};
char *second_row_headers[] = {"Column 1", "Column 2", "Column 3", "Column 4",
                              "Column 5", "Column 6", "Column 7"};

void print_prompt() {
  move(rows - 1, 0);
  printw("solitaire-cli > ");
}

void debug_print_pile(pile *pile, int row, int column) {
  for (int i = 0; i < pile->num_cards; i++) {
    move(row + i, column);
    printw_card(peek_card_at(pile, i));
  }
}

void print_all_curses(game_state *state) {
  // 2 rows, 7 columns
  // top row has a fixed height of 1 card
  // bottom row can have up to 13 cards
  move(0, 0);
  // first row header
  // let's assume 100 characters terminal
  int column_size = 14;
  for (int i = 0; i < 7; i++) {
    move(0, column_size * i);
    printw("%s", first_row_headers[i]);
  }
  pile *stock_pile = stock(state);
  pile *waste_pile = waste(state);
  // first row content
  move(1, 0);
  printw_card(peek(stock_pile));
  move(2, 0);
  printw_pile_size(stock_pile);
  move(1, column_size);
  printw_card(peek_last(waste_pile));
  move(2, column_size);
  printw_pile_size(waste_pile);

  // foundations
  for (int f = 0; f < FOUNDATION_COUNT; f++) {
    int foundation_1_column = 3;
    move(1, (foundation_1_column + f) * column_size);
    printw_card(peek_last(foundation(state, f + 1)));
    move(2, (foundation_1_column + f) * column_size);
    printw_pile_size(foundation(state, f + 1));
  }

  // second row header
  for (int i = 0; i < COLUMN_COUNT; i++) {
    move(4, column_size * i);
    printw("%s", second_row_headers[i]);
    move(5, column_size * i);
    printw_pile_size(column(state, i + 1));
  }

  for (int i = 0; i < COLUMN_COUNT; i++) {
    pile *col = column(state, i + 1);
    int base_row = 6;
    for (int c = 0; c < col->num_cards; c++) {
      move(base_row + c, column_size * i);
      printw_card(peek_card_at(col, c));
    }
  }
  // debug: stock, waste
  mvprintw(17, 0, "stock:");
  debug_print_pile(stock_pile, 18, 0);
  mvprintw(17, 16, "waste:");
  debug_print_pile(waste_pile, 18, 16);
  mvprintw(17, 32, "foundation 1:");
  debug_print_pile(foundation(state, 1), 18, 32);
  mvprintw(17, 48, "foundation 2:");
  debug_print_pile(foundation(state, 2), 18, 48);

  // status bar for the commands
  print_prompt();
}

void prepare_game(game_state *state) {
  pile *stock_pile = stock(state);
  fill_deck(stock_pile);
  shuffle_pile(stock_pile);
  deal(state);
}

typedef struct parsed_input {
  char source;
  char destination;
  int source_index;
  int destination_index;
  int source_amount;
  int success;
} parsed_input;

parsed_input parse_input(char *command) {
  parsed_input parsed;
  parsed.success = 1;
  parsed.source_amount = 1;
  // parser patterns
  char *pattern_multi_move = "%dc%d c%d";
  char *pattern_single_move = "c%d %c%d";
  char *pattern_waste_move = "w %c%d";
  char *pattern_multi_stock = "%ds";
  char *pattern_stock = "s";
  if (sscanf(command, pattern_multi_move, &parsed.source_amount,
             &parsed.source_index, &parsed.destination_index) == 3) {
    parsed.source = 'c';
    parsed.destination = 'c';
  } else if (sscanf(command, pattern_single_move, &parsed.source_index,
                    &parsed.destination, &parsed.destination_index) == 3) {
    parsed.source = 'c';
  } else if (sscanf(command, pattern_waste_move, &parsed.destination,
                    &parsed.destination_index) == 2) {
    parsed.source = 'w';
  } else if (sscanf(command, pattern_multi_stock, &parsed.source_amount) == 1) {
    parsed.source = 's';
  } else if (strcmp(command, pattern_stock) == 0) {
    parsed.source = 's';
  } else {
    parsed.success = 0;
  }
  return parsed;
}

pile *get_pile(game_state *state, char pile_prefix, int pile_index_one_based) {
  switch (pile_prefix) {
  case 's':
    return stock(state);
  case 'w':
    return waste(state);
  case 'f':
    return foundation(state, pile_index_one_based);
  case 'c':
    return column(state, pile_index_one_based);
  default:
    return NULL;
  }
}

enum {
  MOVE_OK,
  MOVE_INVALID_COMMAND,
  MOVE_SOURCE_EMPTY,
  MOVE_INVALID_MOVE,
  MOVE_TOO_MANY_CARDS
};
char *move_results[] = {"OK", "Invalid command", "Source pile empty",
                        "Invalid move", "Too many cards to move!"};

void move_card(card *card, pile *source_pile, pile *destination_pile) {
  delete (source_pile, card);
  reveal(peek_last(source_pile));
  push(destination_pile, card);
}

int attempt_move(game_state *state, char *command) {
  // format: c6 f3
  parsed_input parsed = parse_input(command);
  if (parsed.success != 1) {
    return MOVE_INVALID_COMMAND;
  }

  // figure out destination
  if (parsed.source == 's') {
    for (int i = 0; i < parsed.source_amount; i++) {
      turn(state);
    }
    // TODO remember that the stock can get empty, we need to wrap around
    return MOVE_OK;
  }
  pile *source_pile = get_pile(state, parsed.source, parsed.source_index);
  pile *destination_pile =
      get_pile(state, parsed.destination, parsed.destination_index);

  // check if the move is valid
  if (is_empty(source_pile)) {
    return MOVE_SOURCE_EMPTY;
  }

  if (source_pile->num_cards < parsed.source_amount) {
    return MOVE_TOO_MANY_CARDS;
  }

  int first_card_index = source_pile->num_cards - parsed.source_amount;
  // multi-card move
  if (parsed.source_amount > 1) {
    // check if all cards have been revealed
    card *c = peek_card_at(source_pile, first_card_index);
    if (c->revealed == 0) {
      return MOVE_TOO_MANY_CARDS;
    }
  }

  for (int card_index = 0; card_index < parsed.source_amount; card_index++) {

    // card index doesn't move - the card is 
    card *source_card =
        peek_card_at(source_pile, first_card_index);

    // check if the move is valid based on the destination type
    if (parsed.destination == 'f') {
      // only ace goes if the destination is empty
      if (is_empty(destination_pile) && source_card->rank == RANK_A) {
        move_card(source_card, source_pile, destination_pile);
      }
      if (!is_empty(destination_pile)) {
        // non-empty foundation, pick up the first card
        card *top_foundation_card = peek(destination_pile);
        if (can_be_placed_on_foundation(*top_foundation_card, *source_card)) {
          move_card(source_card, source_pile, destination_pile);
        } else {
          return MOVE_INVALID_MOVE;
        }
      }
    }
    if (parsed.destination == 'c') {
      // king can go in an empty column
      if (is_empty(destination_pile) && source_card->rank == RANK_K) {
        move_card(source_card, source_pile, destination_pile);
      }
      if (!is_empty(destination_pile)) {
        card *bottom_column_card = peek_last(destination_pile);
        if (can_be_placed_bottom(*bottom_column_card, *source_card)) {
          move_card(source_card, source_pile, destination_pile);
        } else {
          return MOVE_INVALID_MOVE;
        }
      }
    }
  }

  // set the return code
  return MOVE_OK;
}

int main() {
  // srand(time(NULL));
  srand(2);
  setlocale(LC_ALL, "");
  init_curses();

  // prepare the game state
  game_state *state = make_game_state();
  prepare_game(state);

  char buffer[80];
  print_all_curses(state);
  // game loop
  while (1) {
    getstr(buffer);

    erase();
    mvprintw(rows - 3, 0, "You entered: %s", buffer);
    // pick up the source, destination and attempt the move
    int result = attempt_move(state, buffer);
    mvprintw(rows - 2, 0, "Move result: %d - %s", result, move_results[result]);
    // show new status in the status bar
    print_all_curses(state);
  }
  getch();
  end_curses();
}
