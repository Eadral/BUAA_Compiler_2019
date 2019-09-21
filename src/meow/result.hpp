//
// Created by rudyc on 2019/9/6.
//

#ifndef BUAAC_RESULT_HPP
#define BUAAC_RESULT_HPP


#include "interface.hpp"
#include "fmt/display.hpp"

#include <any>

namespace meow {

	enum ResultType {
		OK,
		ERR,
	};
	
    template <typename Value, typename Error>
    class Result: implement fmt::Display {
    public:
    	
	    Result(ResultType result_type, Value value) {
			value_ = value;
			result_type_ = result_type;
	    }

    	Result(ResultType result_type, Error err) {
			err_ = err;
			result_type_ = result_type;
	    }

        // Result unwrap() {
        //     if (result_type_ == ResultType::OK) {
        //         return std::any_cast<Result>(value_);
        //     }
        //     panic("unwrap on error");
        // }

		#pragma region COPY_MOVE
    	
	    Result(const Result& other)
		    : fmt::Display(other),
		      result_type_(other.result_type_),
		      value_(other.value_),
		      err_(other.err_) {
	    }

	    Result(Result&& other) noexcept
		    : fmt::Display(std::move(other)),
		      result_type_(other.result_type_),
		      value_(std::move(other.value_)),
		      err_(std::move(other.err_)) {
	    }

	    Result& operator=(const Result& other) {
		    if (this == &other)
			    return *this;
		    fmt::Display::operator =(other);
		    result_type_ = other.result_type_;
		    value_ = other.value_;
		    err_ = other.err_;
		    return *this;
	    }

	    Result& operator=(Result&& other) noexcept {
		    if (this == &other)
			    return *this;
		    fmt::Display::operator =(std::move(other));
		    result_type_ = other.result_type_;
		    value_ = std::move(other.value_);
		    err_ = std::move(other.err_);
		    return *this;
	    }
    	
		#pragma endregion

    	static Result Ok(Value value) {
			return Result(ResultType::OK, value);
	    }

    	static Result Err(Error err) {
			return Result(ResultType::ERR, err);
	    }

		ResultType getType() {
			return result_type_;
	    }
    	
    	Value unwrap() {
		    if (result_type_ == ResultType::OK) {
				return value_;
		    }
			panic("get value on error");
	    }

		Value getValue() {
			return unwrap();
	    }
    	
    	Error getError() {
		    if (result_type_ == ResultType::ERR) {
				return err_;
		    }
			panic("get error on value");
	    }

	    std::string toString() override {
		    if (result_type_ == ResultType::OK) {
				return fmt::Format::toString(value_);
		    } else {
				return fmt::Format::toString(err_);
				// panic("unimplemented");
		    }
	    }

		bool isOk() {
			return result_type_ == ResultType::OK;
	    }
    	
    private:
		ResultType result_type_;
    	
		Value value_;
		Error err_;
    	
    };



}



#endif //BUAAC_RESULT_HPP
