#ifndef __TEE_TUI_API_H__
#define __TEE_TUI_API_H__
#include "tui_global.h"

/* -------------------------------------------- */
#define UNSET  0
#define UNMEASURED  (-654321)
#define INVALID_VAL_PRINT    "invalid"
#define MAX_FILENAME_LEN     1024
#define TEE_STORAGE_PRIVATE 0x00000001  //need change to 1

#define DEFA_SCRN_WIDTH  320
#define DEFA_SCRN_HEIGHT 480


#define DEFAULT_SCREENLABEL_TOP      10
#define DEFAULT_SCREENLABEL_LEFT     10
#define DEFAULT_SCREENLABEL_WIDTH    200
#define DEFAULT_SCREENLABEL_HEIGHT   80
#define DEFAULT_SCREENLABEL_IMAGE    "/usr/share/app-install/icons/agave.png"
#define DEFAULT_ENTRY_LABEL_WIDTH    750
#define DEFAULT_ENTRY_LABEL_HEIGHT   96
#define DEFAULT_BUTTON_WIDTH         300
#define DEFAULT_BUTTON_HEIGHT        150

#define DEFAULT_MARGIN               1
#define DEFAULT_MARGIN_STR          "1"
#define DEFAULT_PADDING              2
#define DEFAULT_PADDING_STR         "2"
#define DEFAULT_BG                  (0x00202020)
#define DEFAULT_BG_STR              "0x00202020"
#define DEFAULT_FG                  (0x00A2A2A2)
#define DEFAULT_FG_STR              "0x00A2A2A2"
#define INVALID_COORD               (0x9FFFFFFF)
#define INVALID_COORD_DEF_STR       "0x9FFFFFFF"

#define DEFAULT_UI_TEXT_ROTATE       0
#define DEFAULT_UI_TEXT_ROTATE_STR  "0"
#define DEFAULT_UI_FONT_SIZE         12  /* not specified in GP */
#define DEFAULT_UI_FONT_SIZE_STR    "12"
#define DEFAULT_UI_FONT_SPACING      2
#define DEFAULT_UI_FONT_SPACING_STR "2"

#define DEFAULT_TEXT_MODE            1    /* TEE_TUI_CLEAR_MODE */
#define DEFAULT_INPUT_TYPE           1    /* TEE_TUI_ALPHANUMERICAL */
#define DELAY_BEFORE_HIDDEN          300  /* milliseconds */
#define NO_LIMIT                     (-1)
#define NO_CARET                     (-1)
#define PASSWD_CHAR                  ('*')

#define DRIFT_DOWN                   3   /* for button: pressed */
#define DRIFT_UP                     1   /* for button: released */
#define TEXT_RECT_SINK               2   /* do not draw */

#define EDITBOX_SPACE				5 //gap + drift + sink

#define INVALID_KEY                 (-3)
#define INVALID_KEY_STR             "-3"
#define INVALID_KEY_VAL             (-4)
#define INVALID_KEY_VAL_STR         "-4"
#define INVALID_IM_INDEX           (-14)
#define INVALID_IM_INDEX_STR       "-14"

#define LOWERCASE                       0
#define UPPERCASE                       1
#define DEFAULT_LETTER_CASE             LOWERCASE
#define SPECIAL_KEY_VAL__BACKSPACE      (0x08)
#define SPECIAL_KEY_VAL__BACKSPACE_STR  "BackSpace"
#define SPECIAL_KEY_VAL__LINEFEED       (0x0A)
#define SPECIAL_KEY_VAL__LINEFEED_STR   "LineFeed"
#define SPECIAL_KEY_VAL__ENTER          (0x0D)
#define SPECIAL_KEY_VAL__ENTER_STR      "Enter"
#define SPECIAL_KEY_VAL__DEL            (0xFF)
#define SPECIAL_KEY_VAL__DEL_STR        "Del"
#define SPECIAL_KEY_VAL__CAPS_LOCK      (-101)
#define SPECIAL_KEY_VAL__CAPS_LOCK_STR  "CAPS_LOCK"
#define SPECIAL_KEY_VAL__SHIFT          (-102)
#define SPECIAL_KEY_VAL__SHIFT_STR      "SHIFT"
#define SPECIAL_KEY_VAL__HIDE_IM        (-103)
#define SPECIAL_KEY_VAL__HIDE_IM_STR    "HIDE_IM"
#define SPECIAL_KEY_VAL_TERPER_LOCK    (-104)



#define DEFAULT_MAX_ENTRY_FIELDS          3   /* account, passwd */
#define DEFAULT_MAX_INPUT_FIELD_LEN       28  /* in chars */
#define DEFAULT_MAX_INPUT_FIELD_LEN_STR  "28"
#define DEFAULT_BUTTON_TEXT_LEN    20

/* refer to section 3.10.4 "Minimum Text Area for Screen Lables": */
#define MIN_LABEL_HEIGHT_IN_CHARS    4  /* lines */
#define MIN_LABEL_WIDTH_IN_CHARS     25 /* chars */

#define DEFAULT_SECURITY_INDICATOR_WIDTH     50
#define DEFAULT_SECURITY_INDICATOR_HEIGHT    50
#define DEFAULT_SECURITY_INDICATOR_BG        (0x00FF1212)
#define DEFAULT_SECURITY_INDICATOR_FG        (0x0012FF12)
#define DEFAULT_SECURITY_INDICATOR_IMAGE     "/usr/share/app-install/icons/akicon.png"  /* solid square */
#define DEFAULT_SECURITY_INDICATOR_IMAGE2    "/usr/share/app-install/icons/xv.png" /* solid square */

/* -------------------------------------------- */
/* color index */
#define RED_IDX   0
#define GREEN_IDX 1
#define BLUE_IDX  2

/* -------------------------------------------- */



/* Global Platform constants & data types */

#define TUI_SESSION_TIMEOUT          10 /* 10 seconds in GP */
#define TEE_ERROR_EXTERNAL_CANCEL    0xFFFF0011
#define TEE_TUI_NUMBER_BUTTON_TYPES  0x00000006

typedef enum {
	TEE_TUI_HIDDEN_MODE = 0,      /* never visible */
	TEE_TUI_CLEAR_MODE,           /* always visible */
	TEE_TUI_TEMPORARY_CLEAR_MODE  /* visible then hidden, delay=? */
} TEE_TUIEntryFieldMode;

typedef enum {
	TEE_TUI_NUMERICAL = 0,
	TEE_TUI_ALPHANUMERICAL
} TEE_TUIEntryFieldType;

typedef enum {
	TEE_TUI_PORTRAIT = 0,
	TEE_TUI_LANDSCAPE
} TEE_TUIScreenOrientation;

typedef enum {
	TEE_TUI_CORRECTION = 0,
	TEE_TUI_OK,
	TEE_TUI_CANCEL,
	TEE_TUI_VALIDATE,
	TEE_TUI_PREVIOUS,
	TEE_TUI_NEXT,
	TEE_TUI_TERMINATE = 0x8001
} TEE_TUIButtonType;

typedef enum {
	TEE_TUI_NO_SOURCE = 0,
	TEE_TUI_REF_SOURCE,
	TEE_TUI_OBJECT_SOURCE,
	TEE_TUI_FILE_SOURCE = 0x8001
} TEE_TUIImageSource;

typedef struct {
	TEE_TUIImageSource source;
	struct {
		/* [inbuf] */
		void *image;
		size_t imageLength;
	} ref;
	struct {
		uint32_t storageID;
		/* [in(objectIDLength)] */
		void *objectID;
		size_t objectIDLen;
	} object;
	uint32_t width;
	uint32_t height;
} TEE_TUIImage;

typedef struct {
	char *text;
	uint32_t textXOffset;         /* top-LEFT */
	uint32_t textYOffset;         /* TOP-left */
	uint8_t textColor[3];         /* 0=Red, 1=Green, 2=Blue */
	TEE_TUIImage image;
	uint32_t imageXOffset;
	uint32_t imageYOffset;
} TEE_TUIScreenLabel;           /* the label for TA branding/mesg, generally on the top of screen */

typedef struct {
	char *text;
	TEE_TUIImage image;
} TEE_TUIButton;


/* this is for ONE tui (one session may contain N TUIs): */
typedef struct {
	TEE_TUIScreenOrientation screenOrientation;
	TEE_TUIScreenLabel label;     /* for TA branding */
	TEE_TUIButton *buttons[TEE_TUI_NUMBER_BUTTON_TYPES];
	bool requestedButtons[TEE_TUI_NUMBER_BUTTON_TYPES];
} TEE_TUIScreenConfiguration;

/* zxf: These buttons can be part of the soft-keyboard! */
typedef struct {
	char *buttonText;
	uint32_t buttonWidth;         /* 0 for non-customnization */
	uint32_t buttonHeight;        /* 0 for non-customnization */
	bool buttonTextCustom;        /* exclusive: below */
	bool buttonImageCustom;       /* exclusive: up */
} TEE_TUIScreenButtonInfo;


/* this is for hardware & button info: */
typedef struct {
	uint32_t grayscaleBitsDepth;
	uint32_t redBitsDepth;
	uint32_t greenBitsDepth;
	uint32_t blueBitsDepth;
	uint32_t widthInch;           /* pixel per inch */
	uint32_t heightInch;          /* pixel per inch */
	uint32_t maxEntryFields;      /* max number of entry fields that can be displayed. */
	uint32_t entryFieldLabelWidth;        /* in pixel */
	uint32_t entryFieldLabelHeight;       /* in pixel */
	uint32_t maxEntryFieldLength; /* max chars for each entry field. */
	uint8_t labelColor[3];        /* 0=Red, 1=Green, 2=Blue */
	uint32_t labelWidth;          /* for canvas */
	uint32_t labelHeight;         /* for canvas */
	TEE_TUIScreenButtonInfo buttonInfo[TEE_TUI_NUMBER_BUTTON_TYPES];
} TEE_TUIScreenInfo;

/* r,g,b: uint8_t */
#define RGB(r,g,b)  ((r<<16) | (g<<8) | b)
#define ARGB(a,r,g,b)  ((a<<24) | (r<<16) | (g<<8) | b)
#define MK_HW_COLOR(scrninfo,red8,green8,blue8)  ({ \
        uint32_t c = 0; \
        TEE_TUIScreenInfo *scrn = (scrninfo); \
        uint8_t r=(red8), g=(green8), b=(blue8); \
        c +=  scrn->redBitsDepth>8   ?r<<(scrn->redBitsDepth   - 8) :r>>(8 - scrn->redBitsDepth); \
        c <<= scrn->greenBitsDepth; \
        c +=  scrn->greenBitsDepth>8 ?g<<(scrn->greenBitsDepth - 8) :g>>(8 - scrn->greenBitsDepth); \
        c <<= scrn->blueBitsDepth; \
        c +=  scrn->greenBitsDepth>8 ?b<<(scrn->greenBitsDepth - 8) :b>>(8 - scrn->greenBitsDepth); \
        c; \
        })


typedef struct {
	char *label;
	TEE_TUIEntryFieldMode mode;
	TEE_TUIEntryFieldType type;
	uint32_t minExpectedLength;
	uint32_t maxExpectedLength;
	/* [outstring] */
	char *buffer;
	size_t bufferLength;
} TEE_TUIEntryField;

/* -------------------------------------------- */

/* -------------------------------------------- */
/* functions  */

TEE_Result TEE_TUIInitSession(void);
TEE_Result TEE_TUICloseSession(void);

/* used to check if a given text can be rendered, and to retrieve w*h needed. */
TEE_Result TEE_TUICheckTextFormat(
	/*[in] */ char *text,
	/*[out] */ uint32_t *width,
	/*[out] */ uint32_t *height,
	/*[out] */ uint32_t *lastIndex
);


TEE_Result TEE_TUIGetScreenInfo(
	/*[in] */ TEE_TUIScreenOrientation screenOrientation,
	/*[in] */ uint32_t nbEntryFields,
	/*[out] */ TEE_TUIScreenInfo *screenInfo
);

TEE_Result TEE_TUIDisplayScreen(
	/*[in] */ TEE_TUIScreenConfiguration *screenConfiguration,
	/*[in] */ bool closeTUISession,
	/*[in] */ TEE_TUIEntryField *entryFields,
	/*[in] */ uint32_t entryFieldCount,
	/*[out] */ TEE_TUIButtonType *selectedButton
);

TEE_Result  TEE_TUISetInfo(int type);
/* -------------------------------------------- */
/* -------------------------------------------- */

#endif
