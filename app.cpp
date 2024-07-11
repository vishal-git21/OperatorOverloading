#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class Node {
public:
    double coefficient;
    int exponent;
    
    Node(double coef, int exp) : coefficient(coef), exponent(exp) {}
};

class Polynomial {
private:
    vector<Node> terms;

public:
    Polynomial() {}

    void addTerm(double coefficient, int exponent) {
        for (auto& term : terms) {
            if (term.exponent == exponent) {
                term.coefficient += coefficient;
                return;
            }
        }
        terms.emplace_back(coefficient, exponent);
        sortTerms();
    }

    void sortTerms() {
        sort(terms.begin(), terms.end(), [](const Node& a, const Node& b) {
            return a.exponent < b.exponent;
        });
    }

    void display() const {
        bool firstTerm = true;
        for (const auto& term : terms) {
            if (term.coefficient == 0) continue;
            if (!firstTerm && term.coefficient > 0) {
                cout << " + ";
            }
            if (term.coefficient < 0) {
                cout << " - ";
            }
            if (abs(term.coefficient) != 1 || term.exponent == 0) {
                cout << abs(term.coefficient);
            }
            if (term.exponent > 0) {
                cout << "x";
                if (term.exponent > 1) {
                    cout << "^" << term.exponent;
                }
            }
            firstTerm = false;
        }
        if (firstTerm) {
            cout << "0";
        }
        cout << endl;
    }

    Polynomial operator+(const Polynomial& p) const {
        Polynomial result;
        int i = 0, j = 0;
        while (i < terms.size() && j < p.terms.size()) {
            if (terms[i].exponent == p.terms[j].exponent) {
                result.addTerm(terms[i].coefficient + p.terms[j].coefficient, terms[i].exponent);
                i++;
                j++;
            } else if (terms[i].exponent > p.terms[j].exponent) {
                result.addTerm(terms[i].coefficient, terms[i].exponent);
                i++;
            } else {
                result.addTerm(p.terms[j].coefficient, p.terms[j].exponent);
                j++;
            }
        }
        while (i < terms.size()) {
            result.addTerm(terms[i].coefficient, terms[i].exponent);
            i++;
        }
        while (j < p.terms.size()) {
            result.addTerm(p.terms[j].coefficient, p.terms[j].exponent);
            j++;
        }
        return result;
    }

    Polynomial operator-(const Polynomial& p) const {
        Polynomial result;
        int i = 0, j = 0;
        while (i < terms.size() && j < p.terms.size()) {
            if (terms[i].exponent == p.terms[j].exponent) {
                double coef = terms[i].coefficient - p.terms[j].coefficient;
                if (coef != 0) {
                    result.addTerm(coef, terms[i].exponent);
                }
                i++;
                j++;
            } else if (terms[i].exponent > p.terms[j].exponent) {
                result.addTerm(terms[i].coefficient, terms[i].exponent);
                i++;
            } else {
                result.addTerm(-p.terms[j].coefficient, p.terms[j].exponent);
                j++;
            }
        }
        while (i < terms.size()) {
            result.addTerm(terms[i].coefficient, terms[i].exponent);
            i++;
        }
        while (j < p.terms.size()) {
            result.addTerm(-p.terms[j].coefficient, p.terms[j].exponent);
            j++;
        }
        return result;
    }

    Polynomial operator*(const Polynomial& p) const {
        Polynomial result;
        for (const auto& term1 : terms) {
            for (const auto& term2 : p.terms) {
                double coef = term1.coefficient * term2.coefficient;
                int exp = term1.exponent + term2.exponent;
                result.addTerm(coef, exp);
            }
        }
        return result;
    }

    Polynomial operator/(const Polynomial& divisor) const {
        Polynomial quotient, remainder(*this);
        while (!remainder.isZero() && remainder.terms.back().exponent >= divisor.terms.back().exponent) {
            double coef = remainder.terms.back().coefficient / divisor.terms.back().coefficient;
            int exp = remainder.terms.back().exponent - divisor.terms.back().exponent;
            Polynomial term;
            term.addTerm(round(coef * 1000) / 1000.0, exp); 
            quotient = quotient + term;
            remainder = remainder - (term * divisor);
        }
        return quotient;
    }

    bool isZero() const {
        for (const auto& term : terms) {
            if (term.coefficient != 0) {
                return false;
            }
        }
        return true;
    }

    void clear() {
        terms.clear();
    }
};

Polynomial inputPolynomial() {
    Polynomial poly;
    int numTerms;
    cout << "Enter the number of terms in the polynomial: ";
    cin >> numTerms;

    for (int i = 0; i < numTerms; ++i) {
        double coef;
        int exp;
        cout << "Enter coefficient for term " << i + 1 << ": ";
        cin >> coef;
        cout << "Enter exponent for term " << i + 1 << ": ";
        cin >> exp;
        poly.addTerm(coef, exp);
    }

    return poly;
}

int main() {
    Polynomial poly1, poly2, result;
    char choice;

    do {
        cout << "Menu:\n";
        cout << "1. Enter Polynomial 1\n";
        cout << "2. Enter Polynomial 2\n";
        cout << "3. Add Polynomials\n";
        cout << "4. Subtract Polynomials\n";
        cout << "5. Multiply Polynomials\n";
        cout << "6. Divide Polynomials\n";
        cout << "7. Display Polynomials\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case '1':
                poly1.clear();
                poly1 = inputPolynomial();
                break;
            case '2':
                poly2.clear();
                poly2 = inputPolynomial();
                break;
            case '3':
                result = poly1 + poly2;
                cout << "Addition Result: ";
                result.display();
                break;
            case '4':
                result = poly1 - poly2;
                cout << "Subtraction Result: ";
                result.display();
                break;
            case '5':
                result = poly1 * poly2;
                cout << "Multiplication Result: ";
                result.display();
                break;
            case '6':
                if (poly2.isZero()) {
                    cout << "Division by zero error. Please enter a non-zero divisor polynomial.\n";
                } else {
                    result = poly1 / poly2;
                    cout << "Division Result: ";
                    result.display();
                }
                break;
            case '7':
                cout << "Polynomial 1: ";
                poly1.display();
                cout << "Polynomial 2: ";
                poly2.display();
                break;
            case '8':
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != '8');

    return 0;
}
