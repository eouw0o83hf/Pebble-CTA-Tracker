#include <pebble.h>
#include "model.h"

//**********************************************
//	Keys

#define KEY_ROUTE_NAME	0
#define KEY_STOP_NAME	1
#define KEY_STOP_DATA	2

#define LISTING_COUNT 10
#define LISTING_HEIGHT 50
	
//	/Keys
//**********************************************

	
//**********************************************
//	Fields	
static Window *s_main_window;
static ScrollLayer *s_scroll_layer;
static TextLayer *s_stop_listings[LISTING_COUNT];

//	/Fields
//**********************************************



//**********************************************
//	Data handling

static void handleUpdate(DictionaryIterator *iterator) {	
	Tuple *routeName = dict_find(iterator, KEY_ROUTE_NAME);	
	Tuple *stopName = dict_find(iterator, KEY_STOP_NAME);	
	Tuple *stopData = dict_find(iterator, KEY_STOP_DATA);
	
	TransitStop* stop = transit_stop_parse(routeName->value->cstring, stopName->value->cstring);
	EstimatedArrivalIterator* parseIterator = get_estimated_arrival_iterator(stopData->value->cstring);
	
	for(int i = 0; i < LISTING_COUNT; ++i) {
		EstimatedArrival* arrival = get_next_estimated_arrival(parseIterator);
		
		if(arrival == NULL) {
			text_layer_set_text(s_stop_listings[i], "");
			continue;
		}
		
		static char buffer[256];
		snprintf(buffer, sizeof(buffer), "Arriving on %s to %s in %d minutes", stop->RouteName, stop->StopName, arrival->Time);
		text_layer_set_text(s_stop_listings[i], buffer);
	}
	
	/*
	GSize max_size = text_layer_get_content_size(s_stop_text);
	text_layer_set_size(s_stop_text, max_size);
 	scroll_layer_set_content_size(s_scroll_layer, GSize(max_size.w, max_size.h));
	*/
}

//	/Data handling
//**********************************************



//**********************************************
//	Data callbacks

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Response received from phone");
	handleUpdate(iterator);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Message dropped");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Outbox send failed");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Outbox send succeeded");
}

//	/Data callbacks
//**********************************************





//**********************************************
//	Main Window open/close

/// Called when the window is loaded
static void main_window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);
	//GRect max_text_bounds = GRect(0, 0, bounds.size.w, 20000);
	
	//int height = 50;
	
	s_scroll_layer = scroll_layer_create(bounds);
	// Bind scrolling to layer
	scroll_layer_set_click_config_onto_window(s_scroll_layer, window);
	
	for(int i = 0; i < LISTING_COUNT; ++i) {
		
		s_stop_listings[i] = text_layer_create(GRect(0, i * LISTING_HEIGHT, 144, LISTING_HEIGHT));
		text_layer_set_background_color(s_stop_listings[i], GColorBlack);
		text_layer_set_text_color(s_stop_listings[i], GColorWhite);
		text_layer_set_text_alignment(s_stop_listings[i], GTextAlignmentLeft);
		text_layer_set_text(s_stop_listings[i], "");
		text_layer_set_font(s_stop_listings[i], fonts_get_system_font(FONT_KEY_GOTHIC_14));

		scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(s_stop_listings[i]));
	}
	
	layer_add_child(window_layer, scroll_layer_get_layer(s_scroll_layer));	
}

/// Called when the window is unloaded
static void main_window_unload(Window *window) {
	scroll_layer_destroy(s_scroll_layer);
	for(int i = 0; i < LISTING_COUNT; ++i) {
		text_layer_destroy(s_stop_listings[i]);		
	}
}

//	/Main Window
//**********************************************




//**********************************************
//	Application start/end
/// App setup
static void init() {
	// SDK best practice: make sure _create() methods
	// in init() are always paired with _destroy()
	// methods in deinit()
	s_main_window = window_create();
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	window_stack_push(s_main_window, true);
	
	// SDK best practice: register callbacks before opening appMessage
	app_message_register_inbox_received(inbox_received_callback);	
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	
	// Now open appMessage, the communication protocol
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

/// App teardown
static void deinit() {
	window_destroy(s_main_window);
}

/// Pretty standard service wireup
int main(void) {	
	APP_LOG(APP_LOG_LEVEL_ERROR, "main");
	init();
	app_event_loop();
	deinit();
}

//	/Application start/end
//**********************************************