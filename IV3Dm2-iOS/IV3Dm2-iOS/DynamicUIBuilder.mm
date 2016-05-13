//  Created by David McCann on 5/12/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "DynamicUIBuilder.h"

@implementation DynamicUIBuilder

-(id) initWitView:(UIView*)view andParameterManipulators:(std::vector<ParameterManipulator*>)manipulators
{
    m_view = view;
    m_manipulators = manipulators;
    m_nameLabels = [[NSMutableArray<UILabel*> alloc] init];
    m_valueLabels = [[NSMutableArray<UILabel*> alloc] init];
    m_steppers = [[NSMutableArray<UIStepper*> alloc] init];
    return self;
}

-(void) generateUI
{
    for (size_t i = 0; i < m_manipulators.size(); ++i) {
        const auto& manipulator = *m_manipulators[i];
        auto floatParams = manipulator.floatParameters();
        for (const auto param : floatParams) {
            UILabel* nameLabel = [self createNameLabel:param];
            [m_nameLabels addObject:nameLabel];
            [m_view addSubview:nameLabel];
            
            UIStepper* stepper = [self createStepper:param withTag:[m_steppers count]];
            [m_steppers addObject:stepper];
            [m_view addSubview:stepper];
            
            UILabel* valueLabel = [self createValueLabel];
            [m_valueLabels addObject:valueLabel];
            [m_view addSubview:valueLabel];
        }
    }
    
    for (UIStepper* stepper in m_steppers) {
        [self stepToValue:stepper];
    }
}

-(void) layoutUI
{
    CGFloat const topMargin = 50;
    CGFloat const leftMargin = 60;
    CGFloat const height = 40;
    CGFloat const spacing = 2;
    
    CGFloat const stepperWidth = 120;
    CGFloat const nameLabelWidth = 120;
    CGFloat const valueLabelWidth = 80;

    for (int i = 0; i < [m_nameLabels count]; ++i) {
        CGFloat top = topMargin + i * (height + spacing);
        m_nameLabels[i].frame = CGRectMake(leftMargin, top, nameLabelWidth, height);
    }
    
    for (int i = 0; i < [m_steppers count]; ++i) {
        CGFloat top = topMargin + i * (height + spacing);
        m_steppers[i].frame = CGRectMake(leftMargin + nameLabelWidth, top, stepperWidth, height);
    }

    for (int i = 0; i < [m_valueLabels count]; ++i) {
        CGFloat top = topMargin + i * (height + spacing);
        m_valueLabels[i].frame = CGRectMake(leftMargin + nameLabelWidth + stepperWidth, top, valueLabelWidth, height);
    }
}

- (void) dealloc
{
    for (UILabel* label in m_nameLabels) {
        [label removeFromSuperview];
    }
    [m_nameLabels removeAllObjects];
    
    for (UIStepper* stepper in m_steppers) {
        [stepper removeFromSuperview];
    }
    [m_steppers removeAllObjects];

    for (UILabel* label in m_valueLabels) {
        [label removeFromSuperview];
    }
    [m_valueLabels removeAllObjects];
}

-(UILabel*) createNameLabel:(const InputParameterFloat&)floatParam
{
    UILabel* label = [[UILabel alloc] init];
    label.text = [NSString stringWithUTF8String:floatParam.name.c_str()];
    label.textColor = [UIColor whiteColor];
    label.backgroundColor = [UIColor colorWithWhite:0 alpha:0];
    label.textAlignment = NSTextAlignmentLeft;
    return label;
}

-(UIStepper*) createStepper:(const InputParameterFloat&)floatParam withTag:(int)tag
{
    UIStepper* stepper = [[UIStepper alloc] init];
    stepper.minimumValue = floatParam.lowerBound;
    stepper.maximumValue = floatParam.upperBound;
    stepper.stepValue = floatParam.stepSize;
    stepper.value = floatParam.defaultValue;
    stepper.tag = tag;
    [stepper addTarget:self action:@selector(stepToValue:) forControlEvents:UIControlEventValueChanged];
    return stepper;
}

-(UILabel*) createValueLabel
{
    UILabel* label = [[UILabel alloc] init];
    label.textColor = [UIColor whiteColor];
    label.backgroundColor = [UIColor colorWithWhite:0 alpha:0];
    label.textAlignment = NSTextAlignmentRight;
    return label;
}

- (void)stepToValue:(UIStepper*)stepper
{
    int index = stepper.tag;
    m_valueLabels[index].text = [NSString stringWithFormat:@"%.02f", stepper.value];
    m_manipulators[0]->setFloatValue("val", stepper.value);
    [[NSNotificationCenter defaultCenter] postNotificationName:@"DatasetChanged" object:self];
}

@end

