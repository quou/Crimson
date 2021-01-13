#pragma once

#include <stddef.h>

#include "core.h"

namespace Crimson {
	class CR_API ref_count {
	private:
		int m_count;
	public:
		ref_count() : m_count(0) {}

		inline void AddRef() {
			++(m_count);
		}

		inline int Release() {
			return --(m_count);
		}
	};

	/* ref - Generic managed heap allocation */
	template <typename T>
	class CR_API ref {
	private:
		ref_count* m_count;
		T* m_ptr;

		inline void release() {
			if (m_count && m_count->Release() == 0) {
				delete m_ptr;
				delete m_count;
			}
		}

		inline void copy(const ref<T>& other) {
			m_ptr = other.m_ptr;
			m_count = other.m_count;
			m_count->AddRef();
		}
	public:
		ref() : m_ptr(NULL), m_count(NULL) {}
		ref(T* o) try : m_ptr(o), m_count(new ref_count()){
			m_count->AddRef();
		} catch (...) {
			delete m_ptr;
			throw;
		}

		/* Copy constructor */
		ref(const ref<T>& other) {
			copy(other);
		}

		virtual ~ref() {
			release();
		}

		ref<T>& operator=(const ref<T>& other) {
			if (other.m_ptr && (this != &other)) {
				release();
				copy(other);
			}
			return *this;
		}

		T& operator*() {
			return *(m_ptr);
		}

		T* operator->() {
			return m_ptr;
		}

		T* operator->() const {
			return m_ptr;
		}

		T* get() {
			return m_ptr;
		}

		T* get() const {
			return m_ptr;
		}

		explicit operator bool() {
			return m_ptr != NULL;
		}

		void reset() const {
			release();
		}
	};
}
