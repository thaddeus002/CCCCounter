/*
    CCCC - C and C++ Code Counter
    Copyright (C) 1994-2005 Tim Littlefair (tim_littlefair@hotmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
*/

/**
 * \file cccc_met.h
 * \brief Definition of class Metric_Treatment and CCCC_Metric
 */


#ifndef __CCCC_MET_H
#define __CCCC_MET_H


#include <string>
#include <fstream>
#include "cccc_itm.h"

/** Alert level based on metric's value */
enum EmphasisLevel { elLOW=0, elMEDIUM=1, elHIGH=2 };


/**
 * The single class CCCC_Metric which will be defined later in this file
 * will be used for all metrics.
 * Differences in output formats will be handled by giving each object
 * of type CCCC_Metric a pointer to a an object of type Metric_Treatment
 * which will be held in a global array called Metric_Treatment_Table
 */
class Metric_Treatment
{
  /**
   * a short code string is used to search for the metric treatment, and
   * it has a full name
   */
  string code, name;

  /**
   * lower_threshold and upper_threshold are the levels at which the metric
   * is interpreted as moving between low, medium and high emphasis levels
   */
  float lower_threshold, upper_threshold;

  /**
   * for ratio type metrics, we provide the facility for screening out of
   * items for which the numerator lies below a given value
   * e.g. we may impose a standard of 1 line of comment per 3 of code, but
   * say that we do not require this standard to apply to routines shorter
   * than 5 lines
   */
  int numerator_threshold;

  /** preferred display width */
  int width;

  /** preferred display number of decimal places */
  int precision;

 public:

  Metric_Treatment(CCCC_Item& treatment_line);
  /** default constructor */
  Metric_Treatment();

  /** write the content of the object in a stream with CCCC_Item format */
  void write(ofstream& optstr);

  /** \return the short code string is used to search for the metric treatment */
  string getCode() {return code; }

  /** \return the full metric name */
  string getName() {return name; }

  /**
   * For the value of the metric, give the emphasis level.
   * \param numerator
   * \param denominator 1 by default
   * \return the emphasis to use for theses values
   */
  EmphasisLevel emphasis_level(int numerator, int denominator=1);

  /**
   * Represent a metric value as a string.
   */
  string value_string(int numerator, int denominator);
};

/**
 * \brief The main metric class, used for all metrics.
 *
 * It has a numerator and a denominator (the metric value is
 * numerator / denominator). The denominator is generally 1.
 *
 * A Metric_Treatment associated with this object define how to show
 * the value (value_string, emphasis).
 */
class CCCC_Metric {
  Metric_Treatment* treatment;
  float numerator, denominator;
  void set_treatment(const char* code);
  void set_ratio(float _num, float _denom=1.0);

 public:
  CCCC_Metric();
  CCCC_Metric(int n, const char* treatment_tag="");
  CCCC_Metric(int n, int d, const char* treatment_tag="");

  /**
   * \return the emphasis to use for the value : normal, warning,
   * or too high.
   */
  EmphasisLevel emphasis_level() const;

  /** \return the short metric code */
  string code() const;

  /** \return the full matric name */
  string name() const;

  /**
   * \return a string that contain the metric value. This string contains
   * only '*' characters for infinite (when the denominator is null), or
   * only '-' characters for non significative value (numerator too low).
   */
  string value_string() const;
};


#endif /* __CCCC_MET_H */

