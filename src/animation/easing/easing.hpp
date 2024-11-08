#pragma once

#ifndef PI
#define PI 3.1415926545
#endif

namespace Entropy::Animation::EasingFunctions {
 /**
  * Applies the 'easeInSine' easing function to a given input time fraction.
  *
  * The 'easeInSine' easing function starts the animation slowly and accelerates
  * as it progresses.
  *
  * @param t The input time fraction, typically between 0.0 and 1.0, where 0.0
  * represents the start of the animation and 1.0 represents the end.
  * @return The transformed time fraction after applying the easing function.
  */
 double easeInSine(double t);

 /**
  * Computes the ease-out sine function for a given time value.
  *
  * @param t The time parameter, typically in the range [0, 1].
  * @return The computed value based on the ease-out sine function.
  */
 double easeOutSine(double t);

 /**
  * Computes the ease-in-out-sine easing function for a given input time.
  *
  * This function smoothly interpolates a value by accelerating and decelerating
  * over the range of [0, 1] based on a sine wave.
  *
  * @param t The input time parameter, typically in the range [0, 1].
  * @return The interpolated value at time t.
  */
 double easeInOutSine(double t);

 /**
  * \brief Computes the quadratic easing-in function.
  *
  * This function implements the quadratic easing-in interpolation, which
  * accelerates motion. The function is defined as t * t.
  *
  * \param t The input parameter, typically representing time, in the range [0, 1].
  * \return The output value after applying the quadratic easing-in function.
  */
 double easeInQuad(double t);

 /**
  * Calculates the ease-out quadratic interpolation of a given time value.
  *
  * @param t The current time, typically a value between 0.0 and 1.0, representing the proportion of the duration that has elapsed.
  * @return The interpolated value at time t.
  */
 double easeOutQuad(double t);

 /**
  * Calculates the ease-in-out quadratic easing function for a given time value.
  *
  * @param t The time value, typically in the range [0, 1].
  * @return The eased value at time t.
  */
 double easeInOutQuad(double t);

 /**
  * Computes the cubic easing-in value for a given normalized time.
  *
  * Easing functions specify the rate of change of a parameter over time. The cubic easing-in function accelerates slowly from zero and then increases speed as time progresses.
  *
  * @param t The normalized time (between 0 and 1).
  * @return The eased value.
  */
 double easeInCubic(double t);

 /**
  * Computes the ease-out cubic interpolation for the given parameter.
  *
  * The ease-out cubic function is defined as:
  * f(t) = 1 + (--t) * t * t
  * This produces a smoother and decelerated motion towards the end of the interval.
  *
  * @param t A double representing the input parameter, typically in the range [0, 1].
  * @return A double representing the interpolated value.
  */
 double easeOutCubic(double t);

 /**
  * Computes the ease-in-out cubic easing function for the given parameter.
  *
  * The easing function smoothly transitions between initial and final values,
  * accelerating during the first half and decelerating during the second half.
  *
  * @param t A double representing the time or position, typically in the range [0, 1].
  * @return A double representing the interpolated value at the given time or position.
  */
 double easeInOutCubic(double t);

 /**
  * @brief Calculates the ease-in quartic interpolation of the given time.
  *
  * This function represents an Easing function where the acceleration from zero velocity is defined by a quartic function.
  *
  * @param t A double representing the current time or position, typically in the range [0, 1], where 0 represents the start and 1 represents the end of the easing.
  * @return A double representing the interpolated value at the given time.
  */
 double easeInQuart(double t);

 /**
  * Applies the ease-out quartic function to the input value.
  *
  * The ease-out quartic function accelerates towards the end of the transition,
  * creating a slow start and fast finish effect.
  *
  * @param t The input value in the range [0, 1] representing the normalized time of the transition.
  * @return The transformed value after applying the ease-out quartic function.
  */
 double easeOutQuart(double t);

 /**
  * Applies the easeInOutQuart easing function to a given time value.
  * This function accelerates during the first half of the interval
  * and decelerates during the second half.
  *
  * @param t The time value, typically in the range [0, 1].
  * @return The eased value.
  */
 double easeInOutQuart(double t);

 /**
  * Calculates the ease-in-quint easing function for a given normalized time.
  *
  * The ease-in-quint function accelerates from zero velocity following a quintic (t^5) curve.
  *
  * @param t The current time, normalized between 0 and 1.
  * @return The eased value at the given time t.
  */
 double easeInQuint(double t);

 /**
  * Eases out with a quintic function.
  *
  * The easeOutQuint function provides a smooth deceleration
  * from the start to the end value, characterized by the
  * polynomial equation: f(t) = 1 - (1-t)^5.
  *
  * @param t The current time, between 0 and 1 inclusive, representing
  *          the progression of the animation or transition.
  *
  * @return The eased value at time t, providing a smooth transition
  *         from 0 to 1.
  */
 double easeOutQuint(double t);

 /**
  * Computes the ease-in-out-quint easing function for a given parameter t.
  *
  * @param t The input value in the range [0, 1], representing the time or position in the animation.
  * @return The eased value corresponding to the input t.
  */
 double easeInOutQuint(double t);

 /**
  * Calculates the exponential easing in function for a given time parameter.
  *
  * @param t The time parameter, typically ranging from 0 to 1.
  * @return The calculated easing value at the given time.
  */
 double easeInExpo(double t);

 /**
  * Eases a value using the exponential out easing function.
  *
  * This function starts fast and then decelerates exponentially towards the end.
  *
  * @param t The input value ranging from 0 to 1, representing the fraction of time elapsed.
  * @return The eased value.
  */
 double easeOutExpo(double t);

 /**
  * Applies an ease-in-out exponential interpolation to the input value.
  *
  * This function performs an exponential interpolation that starts slowly,
  * accelerates, and then slows down again. It is useful for creating smooth and
  * natural transitions.
  *
  * @param t The input value in the range [0, 1], where 0 represents the start and 1 represents the end.
  * @return The interpolated value after applying the ease-in-out exponential function.
  */
 double easeInOutExpo(double t);

 /**
  * @brief Computes the ease-in circular easing function.
  *
  * This function provides a circular easing effect where the rate of change
  * starts slowly and then accelerates over time.
  *
  * @param t The current time, typically between 0 and 1 inclusive.
  * @return The eased value corresponding to the provided time t.
  */
 double easeInCirc(double t);

 /**
  * Computes the circular ease-out value for a given time value.
  *
  * This function implements the circular ease-out formula, which is typically used
  * in animations to create a motion that decelerates towards the end.
  *
  * @param t The current time value, typically between 0 and 1, representing the
  *          progression of the animation.
  * @return The eased value at time t.
  */
 double easeOutCirc(double t);

 /**
  * Calculates the ease-in-out circular easing function at time t.
  *
  * @param t A double representing the current time, typically between 0 and 1.
  * @return A double representing the value of the easing function at time t.
  */
 double easeInOutCirc(double t);

 /**
  * Computes the ease-in-back easing function for a given time value.
  *
  * @param t The current time value, typically between 0 and 1.
  * @return The transformed time value after applying the ease-in-back function.
  */
 double easeInBack(double t);

 /**
  * Calculates the easeOutBack easing value for the given time parameter.
  *
  * This function implements an easing equation that starts by moving
  * backwards before accelerating beyond the target value, and then
  * coming back to the target.
  *
  * @param t A time parameter between 0 and 1 representing the progress
  *          of the animation.
  * @return The eased value corresponding to the given time parameter.
  */
 double easeOutBack(double t);

 /**
  * Applies the ease-in-out-back easing function to the given time value.
  *
  * This function provides a smooth transitioning effect with an overshoot
  * at the beginning and end of the transition. The overshoot magnitude
  * and other characteristic parameters are predetermined within the function.
  *
  * @param t A double representing the current time or position in the easing
  *          duration. Typically, this will range from 0.0 (start) to 1.0 (end).
  * @return The eased value at the given time.
  */
 double easeInOutBack(double t);

 /**
  * Calculates the ease-in-elastic interpolation of a given time value.
  *
  * This function applies the ease-in-elastic easing method to the input time value,
  * producing a smooth elastic effect during the animation start phase.
  *
  * @param t The time value, typically between 0.0 and 1.0, representing the progression
  * of the animation.
  * @return The interpolated value after applying the ease-in-elastic function.
  */
 double easeInElastic(double t);

 /**
  * Computes the easeOutElastic easing function for a given time parameter.
  *
  * The easeOutElastic function starts quickly and decelerates to zero. It produces
  * a large overshoot before settling into its final position.
  *
  * @param t The time parameter, generally in the range [0, 1], where 0 represents
  *          the start and 1 represents the end of the animation.
  * @return The computed value of the easeOutElastic function at time t.
  */
 double easeOutElastic(double t);

 /**
  * Computes the ease-in-out-elastic value for the given parameter.
  *
  * This function applies an elastic easing formula, which combines acceleration and deceleration
  * with a slight overshooting to create a spring-like motion both at the start and at the end.
  * It is commonly used in animations to produce more natural-looking transitions.
  *
  * @param t The input parameter, typically ranging from 0 to 1, representing the progression of the animation.
  * @return A double value representing the eased output for the given input parameter.
  */
 double easeInOutElastic(double t);

 /**
  * Computes the ease-in bounce easing value for the given time.
  *
  * This function provides a bounce effect that accelerates from zero velocity.
  *
  * @param t The current time, typically in the range [0, 1].
  * @return The computed easing value.
  */
 double easeInBounce(double t);

 /**
  * Computes the ease-out bounce easing function for a given time parameter.
  *
  * @param t The time parameter, typically ranging from 0.0 to 1.0, where 0 represents the start time and 1 represents the end time of the animation.
  * @return The computed easing value, which modifies the progression of an animation based on a bouncing effect.
  */
 double easeOutBounce(double t);

 /**
  * Applies an ease-in-out bounce interpolation to a given input parameter.
  *
  * This function takes a parameter `t` which represents the input time and
  * returns a corresponding value that has been interpolated using an
  * ease-in-out bounce function. The function generates a value that progresses
  * from 0 to 1 in a series of bounces, simulating the motion of a bouncing
  * object.
  *
  * @param t The input time parameter in the range [0, 1], where 0 represents the start of the animation and 1 represents the end.
  * @return The interpolated value corresponding to the input time `t`, transformed by the ease-in-out bounce function.
  */
 double easeInOutBounce(double t);
}
