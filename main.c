#include <memory.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

enum {
  SUIT_HEART,
  SUIT_SPADE,
  SUIT_CLUB,
  SUIT_DIAMOND,
  SUIT_COUNT // ♥♠♣♦
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
} card;

typedef card *card_ptr;

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

#endif

#ifdef PILE_ARRAY
typedef struct pile {
  int num_cards;
  card **cards;
} pile;

pile *make_pile() {
  pile *pile = mallocz(sizeof(pile));
  pile->num_cards = 0;
  pile->cards = malloc(sizeof(card *) * CARD_COUNT);
  return pile;
}

pile *make_deck() {
  pile *deck = make_pile();
  for (int rank = 0; rank < RANK_COUNT; rank++) {
    for (int suit = 0; suit < SUIT_COUNT; suit++) {
      deck->cards[deck->num_cards++] = make_card_ptr(suit, rank);
    }
  }
  return deck;
}

void append(pile *pile, card *card) { pile->cards[pile->num_cards++] = card; }

// remove a card from the end of the list
card *pop(pile *pile) {
  card *card = pile->cards[pile->num_cards - 1];
  pile->num_cards--;
  return card;
}

// remove a card from the beginning of the pile
card *dequeue(pile *pile) {
  card *card = pile->cards[0];
  pile->num_cards--;
  // shift all cards to the left
  size_t target_size = (sizeof(card_ptr) * pile->num_cards);
  memmove(&pile->cards, &pile->cards[1], target_size);
  pile->cards = realloc(pile->cards, sizeof(card_ptr) * CARD_COUNT);
  return card;
}

card *peek_card_at(pile *pile, int index) { return pile->cards[index]; }
#endif

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

// append to the end of the list
void push(pile *pile, card *card) {
  card_node *tail = find_tail(pile);
  if (tail == 0) {
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

pile *make_pile() {
  pile *pile_ptr = mallocz(sizeof(pile *));
  pile_ptr->num_cards = 0;
  // pile->cards = malloc(sizeof(card*) * CARD_COUNT);
  return pile_ptr;
}

void fill_deck(pile *pile) {
  for (int rank = 0; rank < RANK_COUNT; rank++) {
          for (int suit = 0; suit < SUIT_COUNT; suit++) {
                  push(pile, make_card_ptr(suit, rank));
          }
  }
  //for (int rank = 0; rank < RANK_8; rank++) {
  //  push(pile, make_card_ptr(SUIT_HEART, rank));
  //}
}
#endif

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
    int idx = rand() % pile->num_cards-1;
    card_ptr card = shift(pile);
    insert(pile, card, idx);
  }
}

#define PRINTCARD(x)                                                           \
  print_card(x);                                                               \
  printf(" ");                                                                 \
  print_card_ptr(x);                                                           \
  printf("\n");

void test_pile_operations(){
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

int main() {
  srand(time(NULL));

  //prepare the game statei
  game_state *state = make_game_state();
  
  // TODO cleanup
  pile *initial_deck = state->piles[PILE_DECK];;
  fill_deck(initial_deck);
  print_deck(initial_deck);
  
  // shuffle
  printf("\nshuffled:\n");
  shuffle_pile(initial_deck);
  print_deck(initial_deck);

}
