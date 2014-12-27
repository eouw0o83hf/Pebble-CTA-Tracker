#include <pebble.h>

//**********************************************
//	Keys

#define KEY_ROUTE_NAME				0
#define KEY_DIRECTION				1
#define KEY_ESTIMATED_ARRIVAL_TIME	2
#define KEY_STOP_NAME				3
#define KEY_TOTAL_FIELD_COUNT		4	// This should match the maximum value,
										// and is used to delimit related data
	
//	/Keys
//**********************************************

	
//**********************************************
//	Fields	
static Window *s_main_window;
static ScrollLayer *s_scroll_layer;
static TextLayer *s_stop_text;

//	/Fields
//**********************************************



//**********************************************
//	Data handling

static void handleUpdate(DictionaryIterator *iterator) {	
	static char route_buffer[4];
	static char direction_buffer[6];
	static char time_buffer[4];
	static char stop_name_buffer[20];	
	static char output_buffer[6400];
		
	for(int i = 0; true; ++i) {
		
		int index = i % KEY_TOTAL_FIELD_COUNT;
		Tuple *item = dict_find(iterator, i);
		
		if(item == NULL) {
			break;
		}
		
		switch(index) {
			case KEY_ROUTE_NAME:
				snprintf(route_buffer, sizeof(route_buffer), "%s", item->value->cstring);
				break;
			
			case KEY_DIRECTION:
				snprintf(direction_buffer, sizeof(direction_buffer), "%s", item->value->cstring);
				break;
			
			case KEY_ESTIMATED_ARRIVAL_TIME:
				snprintf(time_buffer, sizeof(time_buffer), "%s", item->value->cstring);
				break;
			
			case KEY_STOP_NAME:
				snprintf(stop_name_buffer, sizeof(stop_name_buffer), "%s", item->value->cstring);
				break;
		}
		
		if(index == KEY_TOTAL_FIELD_COUNT - 1) {
			// Complete the current round;
			snprintf(output_buffer, sizeof(output_buffer), "%s minutes until next %s-bound bus on %s at %s and also a whole bunch of text that will run off of the end of the page and hopefully make everything scroll Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam quam tellus, fermentu  m quis vulputate quis, vestibulum interdum sapien. Vestibulum lobortis pellentesque pretium. Quisque ultricies purus e  u orci convallis lacinia. Cras a urna mi. Donec convallis ante id dui dapibus nec ullamcorper erat egestas. Aenean a m  auris a sapien commodo lacinia. Sed posuere mi vel risus congue ornare. Curabitur leo nisi, euismod ut pellentesque se  d, suscipit sit amet lorem. Aliquam eget sem vitae sem aliquam ornare. In sem sapien, imperdiet eget pharetra a, lacin  ia ac justo. Suspendisse at ante nec felis facilisis eleifend", 
					 				time_buffer, direction_buffer, route_buffer, stop_name_buffer);
		}
	}
	
	text_layer_set_text(s_stop_text, output_buffer);
	
	GSize max_size = text_layer_get_content_size(s_stop_text);
	text_layer_set_size(s_stop_text, max_size);
 	scroll_layer_set_content_size(s_scroll_layer, GSize(max_size.w, max_size.h));
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
	GRect max_text_bounds = GRect(0, 0, bounds.size.w, 20000);
	
	s_scroll_layer = scroll_layer_create(bounds);
	// Bind scrolling to layer
	scroll_layer_set_click_config_onto_window(s_scroll_layer, window);
	
	s_stop_text = text_layer_create(max_text_bounds);
	text_layer_set_background_color(s_stop_text, GColorBlack);
	text_layer_set_text_color(s_stop_text, GColorWhite);
	text_layer_set_text_alignment(s_stop_text, GTextAlignmentLeft);
	text_layer_set_text(s_stop_text, "");
	text_layer_set_font(s_stop_text, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	
	GSize max_size = text_layer_get_content_size(s_stop_text);
	text_layer_set_size(s_stop_text, max_size);
	
	scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(s_stop_text));
	layer_add_child(window_layer, scroll_layer_get_layer(s_scroll_layer));	
}

/// Called when the window is unloaded
static void main_window_unload(Window *window) {
	scroll_layer_destroy(s_scroll_layer);
	text_layer_destroy(s_stop_text);
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