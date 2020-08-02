
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

