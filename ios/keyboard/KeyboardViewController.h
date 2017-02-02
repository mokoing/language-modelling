//
//  KeyboardViewController.h
//  keyboard
//
//  Created by Devan Kuleindiren on 27/01/2017.
//  Copyright © 2017 Google. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>
#include "tensorflow/Source/lm/rnn/rnn.h"
#include "tensorflow/Source/util/char_trie.h"

@interface KeyboardViewController : UIInputViewController {
    IBOutlet UIButton *firstPrediction;
    IBOutlet UIButton *secondPrediction;
    IBOutlet UIButton *thirdPrediction;
    IBOutlet UIButton *shiftButton;
    NSArray *predictionButtons;
    enum ShiftState {
        LOWER,
        SHIFT,
        UPPER
    };
    ShiftState shiftButtonState;
    
    RNN *rnn;
    bool usePrevStateRNN;
    CharTrie *charTrie;
}

- (void)insertString:(NSString *)s;

- (IBAction)keyPress:(id)sender;
- (IBAction)newLine:(id)sender;
- (IBAction)predictWord:(id)sender;
- (IBAction)backspace:(id)sender;
- (IBAction)nextKeyboard:(id)sender;

@end
