#include <pebble.h>

//**********************************************
//	Fields	
/// The main window
static Window *s_main_window;

//	/Fields
//**********************************************



//**********************************************
//	Data handling

static void handleUpdate(DictionaryIterator *iterator) {
	APP_LOG(APP_LOG_LEVEL_WARNING, "Update received");
	
  	Tuple *t = dict_read_first(iterator);
	while(t != NULL) {
		APP_LOG(APP_LOG_LEVEL_WARNING, "(Key, Value) => (%d, %s)", (int)t->key, t->value->cstring);
		t = dict_read_next(iterator);
	}
	
	APP_LOG(APP_LOG_LEVEL_WARNING, "Done iterating response");
}

//	/Data handling
//**********************************************



//**********************************************
//	Data callbacks

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Response received from phone");
	handleUpdate(iterator);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send succeeded");
}

//	/Data callbacks
//**********************************************





//**********************************************
//	Main Window open/close

/// Called when the window is loaded
static void main_window_load(Window *window) {
	
}

/// Called when the window is unloaded
static void main_window_unload(Window *window) {
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
	init();
	app_event_loop();
	deinit();
}

//	/Application start/end
//**********************************************