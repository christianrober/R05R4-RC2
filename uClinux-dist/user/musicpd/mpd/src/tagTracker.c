#include "tagTracker.h"

#include "list.h"
#include "log.h"

#include <assert.h>

static List * tagLists[TAG_NUM_OF_ITEM_TYPES] =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

typedef struct tagTrackerItem {
	int count;
	mpd_sint8 visited;
} TagTrackerItem;

char * getTagItemString(int type, char * string) {
	ListNode * node;
	int pos;

	/*if(type == TAG_ITEM_TITLE) return strdup(string);*/

	if(tagLists[type] == NULL) {
		tagLists[type] = makeList(free, 1);
		sortList(tagLists[type]);
	}

	if(findNodeInList(tagLists[type], string, &node, &pos)) {
		((TagTrackerItem *)node->data)->count++;
	}
	else {
		TagTrackerItem * item = malloc(sizeof(TagTrackerItem));
		item->count = 1;
		item->visited = 0;
		node = insertInListBeforeNode(tagLists[type], node, pos, 
				string, item);
				
	}

	return node->key;
}

void removeTagItemString(int type, char * string) {
	ListNode * node;
	int pos;

	assert(string);

	assert(tagLists[type]);
	if(tagLists[type] == NULL) return;

	/*if(!node) {
		free(string);
		return;
	}*/

	if(findNodeInList(tagLists[type], string, &node, &pos)) {
		TagTrackerItem * item = node->data;
		item->count--;
		if(item->count <= 0) deleteNodeFromList(tagLists[type], node);
	}

	if(tagLists[type]->numberOfNodes == 0) {
		freeList(tagLists[type]);
		tagLists[type] = NULL;
	}
}

int getNumberOfTagItems(int type) {
	if(tagLists[type] == NULL) return 0;

	return tagLists[type]->numberOfNodes;
}

void sortTagTrackerInfo() {
	int i;

	for(i = 0; i < TAG_NUM_OF_ITEM_TYPES; i++) {
		if(!tagLists[i]) continue;

		DEBUG("sorting %s info\n", mpdTagItemKeys[i]);

		sortList(tagLists[i]);
	}
}

void resetVisitedFlagsInTagTracker(int type) {
	ListNode * node;

	if(!tagLists[type]) return;

	node = tagLists[type]->firstNode;

	while(node) {
		((TagTrackerItem *)node->data)->visited = 0;
		node = node->nextNode;
	}
}

int wasVisitedInTagTracker(int type, char * str) {
	void * item;

	if(!tagLists[type]) return 0;

	if(!findInList(tagLists[type], str, &item)) return 0;

	return ((TagTrackerItem *)item)->visited;
}

void visitInTagTracker(int type, char * str) {
	void * item;

	if(!tagLists[type]) return;

	if(!findInList(tagLists[type], str, &item)) return;

	((TagTrackerItem *)item)->visited = 1;
}

void printVisitedInTagTracker(FILE * fp, int type) {
	ListNode * node;
	TagTrackerItem * item;

	if(!tagLists[type]) return;

	node = tagLists[type]->firstNode;

	while(node) {
		item = node->data;
		if(item->visited) {
			myfprintf(fp, "%s: %s\n", mpdTagItemKeys[type],
					node->key);
		}
		node = node->nextNode;
	}
}
