#ifndef _LIST_H_
#define _LIST_H_

struct list_head {
    struct list_head *next, *prev;
};

#define LIST_LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_LIST_HEAD(name) \
	struct list_head name = LIST_LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); \
	(ptr)->prev = (ptr); \
} while (0)

void list_add     (struct list_head *new, struct list_head *head);
void list_add_tail(struct list_head *new, struct list_head *head);
void list_del     (struct list_head *entry);
void list_del_init(struct list_head *entry);
void list_splice  (struct list_head *list, struct list_head *head);

/**
 *  * list_empty - tests whether a list is empty
 *   * @head: the list to test.
 *    */
static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

static inline void __list_splice(const struct list_head *list,
		struct list_head *prev,
		struct list_head *next)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}

#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))


#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); \
		pos = pos->next)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)



/**
 * list_for_each_entry  -       iterate over list of given type
 * @pos:        the type * to use as a loop cursor.
 * @head:       the head for your list.
 * @member:     the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)                          \
	for (pos = list_entry((head)->next, typeof(*pos), member);      \
			/*prefetch(pos->member.next),*/ &pos->member != (head);        \
			pos = list_entry(pos->member.next, typeof(*pos), member))


/**
 * list_splice_tail - join two lists, each list being a queue
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice_tail(struct list_head *list,
		struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head->prev, head);
}

#endif
