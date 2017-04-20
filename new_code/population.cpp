#include "population.h"
#include "dataset.h"

/****************************************************************************
 * File:        population.cpp
 * Author:      William Gantt
 * Description: Method implementations for the Population class
 ****************************************************************************/ 

using namespace std;

/****************************************************************************
 * Inputs:      r: the rule to be added to the population
 * Outputs:     None.
 * Description: Adds a rule to the population.
 ****************************************************************************/ 
void Population::add(Rule r) {

	// do not add the rule if the population limit has been reached
	if (rules_.size() == max_size_) {
		cout << "Unable to add rule: population limit reached." << endl;
		return;
	}

	// give the rule an ID number (if it doesn't already have one)
	if (r.id() == NO_ID) {
		r.setID(id_count_);
		id_count_++;
	}

	// add the rule to the vector of all rules
	rules_.push_back(r);

	// increment the fitness and experience sums of the population
	// NOTE: currently, no rules being added to the population will
	// have nonzero fitness.
	fitness1_sum_ += r.fitness1();
	fitness2_sum_ += r.fitness2();

} // end add

/****************************************************************************
 * Inputs:      index: the index in the general population of the rule to
 * 		be removed.
 * Outputs:     None.
 * Description: Removes a rule from the population.
 ****************************************************************************/ 
void Population::remove(int index) {

	// if the population is empty, return an error
	if (rules_.size() == 0) {
		printf("Population is empty; rule could not be deleted\n");
		return;
	}

	// decrement the fitness sum of the population
	fitness1_sum_ -= rules_[index].fitness1();
	fitness2_sum_ -= rules_[index].fitness2();

	// Delete the rule. Note that we do not have to delete it from
	// the match or correct sets because those are reset at every
	// iteration anyway. 
	rules_.erase(rules_.begin() + index);

} // end remove

/****************************************************************************
 * Inputs:      None.
 * Outputs:     The index in the population of the selected rule.
 * Description: Selects a rule from the population using roulette wheel
 * 		selection---a fitness-based selection algorithm.
 ****************************************************************************/
int Population::rouletteWheelSelect() {

	// select a random number in the range [0,correct_set_.fitness_sum()]
	double random = real_dist(rng) * fitness2_sum();

	/*
	 * we determine the individual selected by subtracting
	 * the individual fitnesses from the random value generated
	 * above until that value falls to or below 0. In this way,
	 * an individual's likelihood of being selected is directly
	 * proportional to its fitness
	 */
	int num_rules = rules_.size();
	for (int i=0; i<num_rules; i++) {
		random -= rules_[i].fitness2();
		if (random <= 0) {
			return i;
		}
	}

	// control should never reach here
	return (num_rules - 1);

} // end rouletteWheelSelect

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Computes fitness1 for all rules in the population.
 ****************************************************************************/ 
void Population::evaluateFitness1() {

	// reset the fitness1 sum
	fitness1_sum_ = 0;

	// get the number of rules
	int num_examples = training_set_.num_data_points();

	// a variable to store the current example
	vector<double> curr_example;

	// iterate over all the rules
	for (int i=0; i<max_size_; i++) {

		// for each input, determine whether it is a true positive,
		// true negative, false positive, or false negative for the
		// current rule
		for (int j=0; j<num_examples; j++) {
			curr_example = training_set_.data_points_[j];
			rules_[i].processInput(curr_example);	
		}

		// once the number of true and false positives and negatives
		// has been determined, we update the fitness of the rule,
		// and the fitness1 sum for the population
		rules_[i].updateFitness1();
		fitness1_sum_ += rules_[i].fitness1();
	}

} // end evaluateFitness1

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Computes fitness2 for all rules in the population.
 ****************************************************************************/ 
void Population::evaluateFitness2() {

	// the population size and the number of examples
	int num_examples = training_set_.num_data_points();

	// the classification of an example for the current rule (true
	// positive, false positive, true negative, or false negative)
	int example_type = 0;

	// reset the array that keeps track of which examples have already
	// been covered by a rule, as well as the fitness2 sum
	reset();

	// rank the rules by fitness1
	rankByFitness1();

	// iterate over the set of rules in rank order
	for (int i=0; i<max_size_; i++) {

		// reset TP, TN, FP, FN to 0.5 (they were already counted once
		// when we computed fitness1, so they have to be reset here
		// for the second count)
		rules_[i].resetCounts();

		// iterate over the set of examples
		for (int j=0; j<num_examples; j++) {

			// run processInput for the current example and
			// the current rule
			example_type = rules_[i].processInput(training_set_.data_points_[j]);

			// if current example is a TP for the current rule
			if (example_type == TP) {

				// if the current example has NOT already been covered by
				// another rule, indicate that the example has now been
				// covered (by this rule)
				if (training_set_.examples_covered_[j] == false) {
					training_set_.examples_covered_[j] = true;

				// if the current example HAS already been covered, we have
				// to decrement the true positive count of the current rule.
				// This is because the count was incremented when processInput
				// was called above
				} else {
					double num_tp = rules_[i].true_positives();
					rules_[i].setTruePositives(num_tp - 1);
				}
			}
		}
		
		// now that we know the counts of true and false positives for this rule,
		// we can update fitness2 and add the result to the fitness2 sum
		rules_[i].updateFitness2();
		fitness2_sum_ += rules_[i].fitness2();
	}

} // evaluateFitness2

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 *
 * NEED BETTER COMMENTS HERE
 *
 * Note: (Does the population have to be ranked by fitness2 for this to
 * 	  work properly?)
 ****************************************************************************/ 
vector<int> Population::sus(int num_to_select) {

	// a vector for storing the indices of the selected rules
	vector<int> selected(num_to_select);
	if ((fitness2_sum_ > 0) && (num_to_select < max_size_)) {

		// get the pointer interval
		double pointer_interval = fitness2_sum_ / num_to_select;

		// select a random number in the range [0,pointer_interval]
		double r = fmod(rng(),pointer_interval);

		// select the parents
		for (int i=0; i<num_to_select; i++) {
			int j = 0;
			double curr_fitness2_sum = 0;
			while (curr_fitness2_sum < (r + (i * pointer_interval))) {
				curr_fitness2_sum += rules_[j].fitness2();
				j++;
			}
			selected[i] = j-1;
		}
	} 

	return selected;

} // end sus

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: 
 *
 * NOTE: Must guarantee that there are an even number of parents!
 ****************************************************************************/
void Population::applyGA() {

	// evaluate fitnesses
	evaluateFitness1();
	evaluateFitness2();

	// by ranking by fitness2, we put the elite rules for the new generation
	// in the correct position
	rankByFitness2();

	// the number of rules from the previous generation that are
	// to be preserved
	int num_elites = round(max_size_ * elitism_rate_);
	int rest_of_pop = max_size_ - num_elites;

	// select the rules to be used for crossover
	vector<int> selected = sus(rest_of_pop);

	// Perform Xover and mutation (note that this is being performed
	// on the previous generation and NOT new_pop, which is as it should
	// be)
	vector<Rule> offspring = crossoverAndMutate(selected);

	// select offspring at random to fill the remaining slots in the
	// population
	random_shuffle(offspring.begin(), offspring.end());
	offspring.resize(rest_of_pop);

	// copy the selected offspring into the new population
	for (int i=0; i<rest_of_pop; i++) {
		offspring[i].setID(rules_[num_elites + i].id());
		rules_[num_elites + i] = offspring[i];
	}

} // end select

/****************************************************************************
 * Inputs:      A vector containing the indices of the rules that have
 * 		been selected for crossover and mutation.
 * Outputs:     The mutated and crossed-over rules.
 * Description: Applies the crossover and mutation operators to a subset of
 * 		the population.
 ****************************************************************************/
vector<Rule> Population::crossoverAndMutate(vector<int> selected) {
	
	// shuffle the vector of selected rule indices
	random_shuffle(selected.begin(),selected.end());

	// the number of offspring
	int num_offspring = selected.size();

	// stores a pair of offspring from a single crossover
	pair<Rule,Rule> offspring;

	// a vector to contain the offspring rules
	vector<Rule> new_rules(num_offspring);

	// iterate through the vector, calling crossover on pairs of
	// rules and mutating the offspring
	for (int i=0; i<num_offspring; i+=2) {

		// crossover the current set of parents
		offspring = crossover(selected[i],selected[i+1]);

		// mutate the children
		offspring.first.mutate(mutate_prob_,dont_care_prob_,
				training_set_.attribute_quantiles_);
		offspring.second.mutate(mutate_prob_,dont_care_prob_,
				training_set_.attribute_quantiles_);
		
		// add the children to the vector of new rules to be returned
		new_rules[i] = offspring.first;
		new_rules[i+1] = offspring.second;
	}

	return new_rules;

} // end crossoverAndMutate

/****************************************************************************
 * Inputs:      i,j: the indices in the general population of the rules
 * 		to be crossed.
 * Outputs:     A pair of offspring rules
 * Description: Creates two new rules via one-point crossover of the rules
 * 		specified by the values i and j.
 ****************************************************************************/ 
pair<Rule,Rule> Population::crossover(int i, int j) {
	
	// get the parents
	Rule p1 = rules_[i];
	Rule p2 = rules_[j];

	// select a point in [0,condition.size() - 1] for 1-pt crossover
	int cross_point = (rng() % p1.condition_.size());

	// create the offspring
	Rule off1 = Rule();
	Rule off2 = Rule();

	// copy parent conditions into offspring (up to crossover point)
	for (int i=0; i<cross_point; i++) {

		// add the appropriate attribute values to the rules
		off1.condition_.push_back(p1.condition_[i]);
		off2.condition_.push_back(p2.condition_[i]);

		// update the number of "don't cares," if necessary
		if (p1.condition_[i].dont_care())
			off1.setNumDontCare(off1.num_dont_care() + 1);
		if (p2.condition_[i].dont_care())
			off2.setNumDontCare(off2.num_dont_care() + 1);
	}

	// copy parent conditions into offspring (after crossover point)
	for (int i=cross_point; i<p1.condition_.size(); i++) {

		off1.condition_.push_back(p2.condition_[i]);
		off2.condition_.push_back(p1.condition_[i]);

		if (p1.condition_[i].dont_care())
			off2.setNumDontCare(off2.num_dont_care() + 1);
		if (p2.condition_[i].dont_care())
			off1.setNumDontCare(off1.num_dont_care() + 1);
	}

	// copy classes
	off1.setClass(p1.classification());
	off2.setClass(p2.classification());

	// make sure that the fitnesses are set to 0
	off1.setFitness1(0);
	off2.setFitness1(0);

	// return the pair
	return make_pair(off1, off2);

} // end crossover

/****************************************************************************
 * Input:       
 * Output:      
 * Description: 
 ****************************************************************************/ 
Population Population::random(int pop_size,
				int num_iters,
				int target_class,
				int default_class,
				double elitism_rate,
				double mutate_prob,
				double dont_care_prob,
				Dataset training_set, 
				Dataset test_set) {

	// initialize the population of rules
	Population p = Population(pop_size, num_iters, target_class, default_class, elitism_rate, 
				  mutate_prob, dont_care_prob, training_set, test_set);

	// a rule variable for generating random rules
	Rule r;

	// the number of class attribute values
	int num_classes = p.training_set_.num_classes();

	// the quantiles for the target class
	vector<vector<double> > target_class_quantiles = p.training_set_.attribute_quantiles_;

	// generate an initial population of random rules
	for (int i=0; i<pop_size; i++) {

		// generate a random rule and add it to the population
		r = Rule::random(num_classes, target_class_quantiles, p.dont_care_prob());
		r.setID(p.id_count_);
		r.setClass(p.target_class());
		p.add(r);
		p.id_count_++;
	}

	return p;

} // end random

/****************************************************************************
 * Input:       
 * 		default_class: the default class to be assigned to an
 * 		example if no matching rule is found.
 * Output:      The fraction of examples that were correctly classified.
 * Description: Classifies examples from the test set based on the rules
 * 		in the current population.
 *
 * Note:        The default class and the target class MUST be different.
 * 		Otherwise, all examples will be classified as the target
 * 		class. 
 ****************************************************************************/ 
double Population::classify(Dataset* d, string output_file = "") {

	// The class chosen for the current example
	int selected_class = NO_CLASS;

	// the number of true and false positives and negatives
	double tp = 0;
	double tn = 0;
	double fp = 0;
	double fn = 0;

	// the number of examples in the target class
	int target_class_size = 0;

	double accuracy = 0;
	double odds_ratio = 0;
	
	// rank the population by fitness2
	rankByFitness2();

	// the number of rules retained through elitism
	int num_elites = round(max_size_ * elitism_rate_);

	// in the classification, we want to use only those rules that
	// correctly identify at least one positive example, and that
	// have more true positives than false positives
	vector<int> rules_to_use;
	for (int i=0; i<num_elites; i++) {

		if ((rules_[i].true_positives() > 0.5) && 
		    (rules_[i].true_positives() > rules_[i].false_positives()))
			rules_to_use.push_back(rules_[i].id());

	}

	// the number of rules that will be used in the classification
	num_elites = rules_to_use.size();

	// iterate over the examples in the test set
	int data_set_size = d->data_points_.size();
	for (int i=0; i<data_set_size; i++) {
	
		// get the current example
		vector<double> curr_ex = d->data_points_[i];

		// iterate over the rules in the population until either:
		// 	1. A rule is found that matches the example
		// 	2. There are no more rules to consider
		int rule_counter = 0;
		while ((rule_counter < num_elites) && 
		       (!rules_[rule_counter].matches(curr_ex)))
			rule_counter++;	
		
		// if the rule counter has the same value as the size of the
		// population, no matching rule was found
		if (rule_counter == num_elites) {
			selected_class = default_class_;

		// otherwise, a matching rule must have been found
		} else {
			selected_class = rules_[rule_counter].classification();
		}

		// 
		// determine whether the rule is a true positive, 
		// false positive, true negative, or false negative
		//

		// current example matches the target class
		if (curr_ex.back() == target_class_) {

			// increment the size of the target class
			target_class_size++;

			// the chosen class also matches the target class
			if (selected_class == target_class_)
				tp++; // then it's a true positive
			else
				fn++; // otherwise, it's a false negative

		// current example does not match the target class
		} else {

			// but the chosen class matches the target class
			if (selected_class == target_class_)
				fp++; // then it's a false positive
			else
				tn++; // otherwise, it's a true negative
		}
	}	

	// WRITE OUTPUT
	if (!output_file.empty()) {

		fstream file_stream;

		// the flags for the open function here indicate that
		// we want to *append* to the existing contents of the file
		file_stream.open(output_file.c_str(), fstream::in | fstream::out | fstream::app);

		// if file opened successfully, start writing
		if (file_stream.good()) {

			file_stream << endl << endl;

			// number of rules in the target class and
			// true and false positives and negatives
			file_stream << "Target Class Size: " << target_class_size << endl;
			file_stream << "True Positives:    " << tp << endl;
			file_stream << "True Negatives:    " << tn << endl;
			file_stream << "False Positives:   " << fp << endl;
			file_stream << "False Negatives:   " << fn << endl << endl;

			// lists which rules were actually used on the test set
			file_stream << "Rules used for classification: ";
			sort(rules_to_use.begin(), rules_to_use.end());
			for (int i=0; i<num_elites; i++) {
				file_stream << rules_to_use[i];
			        if (i<(num_elites - 1))
					file_stream << ", ";
			}	

			file_stream << endl;

			// testing accuracy and testing odds ratio
			accuracy = (tp + tn) / (tp + tn + fp + fn);
			odds_ratio = ((tp + 0.5) + (tn + 0.5)) / ((fp + 0.5) + (fn + 0.5));
			file_stream << "Accuracy:          " << accuracy << endl;
			file_stream << "Odds Ratio:        " << odds_ratio << endl;
		}
		// close the file
		file_stream.close();
	}

	// print some statistics to stdout for immediate viewing
	printf("target class: %d\n", target_class_);
	printf("target class size: %d\n", target_class_size);
	printf("num elites: %d\n", num_elites);
	printf("TP: %.3f\n", tp);
	printf("TN: %.3f\n", tn);
	printf("FP: %.3f\n", fp);
	printf("FN: %.3f\n", fn);

	return accuracy;

} // end classify


/****************************************************************************
 * Inputs:      filename: The file to be written to
 * Outputs:     None.
 * Description: Writes information about a single run of the LCS to a file.
 ****************************************************************************/ 
void Population::writeRunData(std::string training_file,
		              std::string testing_file,
			      std::string quantiles_file,
			      std::string output_file) {

	// attempt to open the file
	ofstream file_stream;
	file_stream.open(output_file.c_str());
	
	// if the file opened correctly, start writing
	if (file_stream.good()) {

		// population parameters
		file_stream << "Population Parameters" << endl;
	        file_stream << "---------------------" << endl << endl;
		file_stream << "Size:            " << max_size_ << endl;
		file_stream << "Iterations:      " << num_iters_ << endl;
		file_stream << "Target Class:    " << target_class_ << endl;
		file_stream << "Default Class:   " << default_class_ << endl;
		file_stream << "Mutation Prob:   " << mutate_prob_ << endl;
		file_stream << "Don't Care Prob: " << dont_care_prob_ << endl;
		file_stream << "Elitism Rate:    " << elitism_rate_ << endl << endl << endl;

		// final population
		file_stream << "Final Population" << endl;
		file_stream << "----------------" << endl << endl;

		// this makes it easier to see the fittest rules
		rankByFitness2();

		// print every rule in the population
		int condition_length = rules_[0].condition_.size();
		char dc[] = "[DC]";
		for (int i=0; i<max_size_; i++) {

			Rule r = rules_[i];

			// rule number
			file_stream << "Rule " << r.id() << endl;
			file_stream << "-------" << endl;

			// attribute number
			file_stream << "Attribute:     ";
			for (int j=0; j<condition_length; j++) {
				file_stream << setw(7);
				file_stream << j; 
			}

			// don't care values
			file_stream << endl << "Don't Care:     ";
			for (int j=0; j<condition_length; j++) {
				file_stream << setw(7);
				if (r.condition_[j].dont_care())
					file_stream << dc;
				else
					file_stream << " ";
			}

			// lower bound
			file_stream << endl << "Lower Bound:     ";
			for (int j=0; j<condition_length; j++) {
				file_stream << setw(7) << setprecision(3);
				if (!r.condition_[j].dont_care())
					file_stream << r.condition_[j].l_bound();
				else
					file_stream << " ";

			}

			// upper bound
			file_stream << endl << "Upper Bound:     ";
			for (int j=0; j<condition_length; j++) {
				file_stream << setw(7) << setprecision(3);
				if (!r.condition_[j].dont_care())
					file_stream << r.condition_[j].u_bound();
				else
					file_stream << " ";

			}

			// quantile
			file_stream << endl << "Quantile:        ";
			for (int j=0; j<condition_length; j++) {
				file_stream << setw(7) << setprecision(3);
				if (!r.condition_[j].dont_care())
					file_stream << r.condition_[j].quantile();
				else
					file_stream << " ";

			}

			// other rule info
			file_stream << endl << endl;
			file_stream << "Class:        " << r.classification() << endl;
			file_stream << "# Don't Care: " << r.num_dont_care() << endl;
			file_stream << "Fitness 1:    " << r.fitness1() << endl;
			file_stream << "Fitness 2:    " << r.fitness2() << endl;
			file_stream << "True Pos:     " << r.true_positives() << endl;
			file_stream << "True Neg:     " << r.true_negatives() << endl;
			file_stream << "False Pos:    " << r.false_positives() << endl;
			file_stream << "False Neg:    " << r.false_negatives() << endl;
			file_stream << endl << endl;
		}

		// training and testing files
		file_stream << "Training and Testing Data" << endl;
		file_stream << "-------------------------" << endl << endl;
		file_stream << "Training File:  " << training_file << endl;
		file_stream << "Testing File:   " << testing_file << endl;
		file_stream << "Quantiles File: " << quantiles_file << endl; 

		// NOTE: data about classification accuracy and odds ratio and
		// so forth are appended to the file by the classify function

	}

	file_stream.close();

} // end writeRunData

