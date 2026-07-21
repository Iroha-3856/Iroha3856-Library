namespace itertools {

namespace detail {

template <class T, std::ranges::input_range R>
vector<T> make_pool(const R& range) {
    vector<T> pool;
    if constexpr (std::ranges::sized_range<R>) pool.reserve(std::ranges::size(range));
    for (const auto& x : range) pool.push_back(x);
    return pool;
}

template <class T>
class CombinationsRange {
    vector<T> pool;
    size_t r;

public:
    CombinationsRange(vector<T> pool_, ptrdiff_t r_) : pool(move(pool_)) {
        if (r_ < 0) throw invalid_argument("combinations(): r must be non-negative");
        r = static_cast<size_t>(r_);
    }

    class Iterator {
        const vector<T>* pool = nullptr;
        vector<size_t> indices;
        vector<T> current;
        bool done = true;

        void update_current() {
            for (size_t i = 0; i < indices.size(); i++) current[i] = (*pool)[indices[i]];
        }

    public:
        using iterator_category = input_iterator_tag;
        using value_type = vector<T>;
        using difference_type = ptrdiff_t;

        Iterator() = default;

        Iterator(const vector<T>* pool_, size_t r_) : pool(pool_), indices(r_), current(r_), done(r_ > pool_->size()) {
            if (done) return;
            iota(indices.begin(), indices.end(), 0);
            update_current();
        }

        const value_type& operator*() const {
            return current;
        }

        const value_type* operator->() const {
            return &current;
        }

        Iterator& operator++() {
            if (done) return *this;

            const size_t n = pool->size();
            const size_t r = indices.size();

            if (r == 0) {
                done = true;
                return *this;
            }

            size_t i = r;
            while (i > 0 && indices[i - 1] == n - r + i - 1) i--;

            if (i == 0) {
                done = true;
                return *this;
            }

            i--;
            indices[i]++;

            for (size_t j = i + 1; j < r; j++) indices[j] = indices[j - 1] + 1;

            update_current();
            return *this;
        }

        void operator++(int) {
            ++*this;
        }

        friend bool operator==(const Iterator& it, default_sentinel_t) {
            return it.done;
        }

        friend bool operator==(default_sentinel_t, const Iterator& it) {
            return it.done;
        }

        friend bool operator!=(const Iterator& it, default_sentinel_t) {
            return !it.done;
        }

        friend bool operator!=(default_sentinel_t, const Iterator& it) {
            return !it.done;
        }
    };

    Iterator begin() const {
        return Iterator(&pool, r);
    }

    default_sentinel_t end() const {
        return {};
    }
};

template <class T>
class PermutationsRange {
    vector<T> pool;
    size_t r;

public:
    PermutationsRange(vector<T> pool_, ptrdiff_t r_) : pool(move(pool_)) {
        if (r_ < 0) throw invalid_argument("permutations(): r must be non-negative");
        r = static_cast<size_t>(r_);
    }

    class Iterator {
        const vector<T>* pool = nullptr;
        vector<size_t> indices;
        vector<size_t> cycles;
        vector<T> current;
        size_t r = 0;
        bool done = true;

        void update_current() {
            for (size_t i = 0; i < r; i++) current[i] = (*pool)[indices[i]];
        }

    public:
        using iterator_category = input_iterator_tag;
        using value_type = vector<T>;
        using difference_type = ptrdiff_t;

        Iterator() = default;

        Iterator(const vector<T>* pool_, size_t r_) : pool(pool_), indices(pool_->size()), cycles(r_), current(r_), r(r_), done(r_ > pool_->size()) {
            if (done) return;

            iota(indices.begin(), indices.end(), 0);
            for (size_t i = 0; i < r; i++) cycles[i] = pool->size() - i;

            update_current();
        }

        const value_type& operator*() const {
            return current;
        }

        const value_type* operator->() const {
            return &current;
        }

        Iterator& operator++() {
            if (done) return *this;

            const size_t n = pool->size();

            if (r == 0) {
                done = true;
                return *this;
            }

            for (size_t j = r; j > 0; j--) {
                const size_t i = j - 1;

                cycles[i]--;

                if (cycles[i] == 0) {
                    rotate(indices.begin() + i, indices.begin() + i + 1, indices.end());
                    cycles[i] = n - i;
                } else {
                    swap(indices[i], indices[n - cycles[i]]);
                    update_current();
                    return *this;
                }
            }

            done = true;
            return *this;
        }

        void operator++(int) {
            ++*this;
        }

        friend bool operator==(const Iterator& it, default_sentinel_t) {
            return it.done;
        }

        friend bool operator==(default_sentinel_t, const Iterator& it) {
            return it.done;
        }

        friend bool operator!=(const Iterator& it, default_sentinel_t) {
            return !it.done;
        }

        friend bool operator!=(default_sentinel_t, const Iterator& it) {
            return !it.done;
        }
    };

    Iterator begin() const {
        return Iterator(&pool, r);
    }

    default_sentinel_t end() const {
        return {};
    }
};

template <class T>
class ProductRange {
    vector<vector<T>> pools;

public:
    ProductRange(vector<vector<T>> pools_, ptrdiff_t repeat) {
        if (repeat < 0) throw invalid_argument("product(): repeat must be non-negative");

        if (repeat != 0 && pools_.size() > numeric_limits<size_t>::max() / static_cast<size_t>(repeat)) {
            throw length_error("product(): too many pools");
        }

        pools.reserve(pools_.size() * static_cast<size_t>(repeat));

        for (ptrdiff_t i = 0; i < repeat; i++) {
            for (const auto& pool : pools_) pools.push_back(pool);
        }
    }

    class Iterator {
        const vector<vector<T>>* pools = nullptr;
        vector<size_t> indices;
        vector<T> current;
        bool done = true;

        void update_current() {
            for (size_t i = 0; i < indices.size(); i++) current[i] = (*pools)[i][indices[i]];
        }

    public:
        using iterator_category = input_iterator_tag;
        using value_type = vector<T>;
        using difference_type = ptrdiff_t;

        Iterator() = default;

        explicit Iterator(const vector<vector<T>>* pools_) : pools(pools_), indices(pools_->size(), 0), current(pools_->size()), done(false) {
            for (const auto& pool : *pools) {
                if (pool.empty()) {
                    done = true;
                    return;
                }
            }

            update_current();
        }

        const value_type& operator*() const {
            return current;
        }

        const value_type* operator->() const {
            return &current;
        }

        Iterator& operator++() {
            if (done) return *this;

            const size_t n = pools->size();

            if (n == 0) {
                done = true;
                return *this;
            }

            for (size_t j = n; j > 0; j--) {
                const size_t i = j - 1;

                indices[i]++;

                if (indices[i] < (*pools)[i].size()) {
                    update_current();
                    return *this;
                }

                indices[i] = 0;
            }

            done = true;
            return *this;
        }

        void operator++(int) {
            ++*this;
        }

        friend bool operator==(const Iterator& it, default_sentinel_t) {
            return it.done;
        }

        friend bool operator==(default_sentinel_t, const Iterator& it) {
            return it.done;
        }

        friend bool operator!=(const Iterator& it, default_sentinel_t) {
            return !it.done;
        }

        friend bool operator!=(default_sentinel_t, const Iterator& it) {
            return !it.done;
        }
    };

    Iterator begin() const {
        return Iterator(&pools);
    }

    default_sentinel_t end() const {
        return {};
    }
};

template <class T>
class CombinationsWithReplacementRange {
    vector<T> pool;
    size_t r;

public:
    CombinationsWithReplacementRange(vector<T> pool_, ptrdiff_t r_) : pool(move(pool_)) {
        if (r_ < 0) throw invalid_argument("combinations_with_replacement(): r must be non-negative");
        r = static_cast<size_t>(r_);
    }

    class Iterator {
        const vector<T>* pool = nullptr;
        vector<size_t> indices;
        vector<T> current;
        bool done = true;

        void update_current() {
            for (size_t i = 0; i < indices.size(); i++) current[i] = (*pool)[indices[i]];
        }

    public:
        using iterator_category = input_iterator_tag;
        using value_type = vector<T>;
        using difference_type = ptrdiff_t;

        Iterator() = default;

        Iterator(const vector<T>* pool_, size_t r_) : pool(pool_), indices(r_, 0), current(r_), done(pool_->empty() && r_ > 0) {
            if (!done) update_current();
        }

        const value_type& operator*() const {
            return current;
        }

        const value_type* operator->() const {
            return &current;
        }

        Iterator& operator++() {
            if (done) return *this;

            const size_t r = indices.size();

            if (r == 0) {
                done = true;
                return *this;
            }

            const size_t n = pool->size();

            size_t i = r;
            while (i > 0 && indices[i - 1] == n - 1) i--;

            if (i == 0) {
                done = true;
                return *this;
            }

            i--;

            const size_t next = indices[i] + 1;
            for (size_t j = i; j < r; j++) indices[j] = next;

            update_current();
            return *this;
        }

        void operator++(int) {
            ++*this;
        }

        friend bool operator==(const Iterator& it, default_sentinel_t) {
            return it.done;
        }

        friend bool operator==(default_sentinel_t, const Iterator& it) {
            return it.done;
        }

        friend bool operator!=(const Iterator& it, default_sentinel_t) {
            return !it.done;
        }

        friend bool operator!=(default_sentinel_t, const Iterator& it) {
            return !it.done;
        }
    };

    Iterator begin() const {
        return Iterator(&pool, r);
    }

    default_sentinel_t end() const {
        return {};
    }
};

} // namespace detail

template <std::ranges::input_range R>
auto combinations(const R& range, ptrdiff_t r) {
    using T = remove_cv_t<std::ranges::range_value_t<R>>;
    return detail::CombinationsRange<T>(detail::make_pool<T>(range), r);
}

template <std::ranges::input_range R>
auto permutations(const R& range, ptrdiff_t r) {
    using T = remove_cv_t<std::ranges::range_value_t<R>>;
    return detail::PermutationsRange<T>(detail::make_pool<T>(range), r);
}

template <std::ranges::input_range R>
auto permutations(const R& range) {
    using T = remove_cv_t<std::ranges::range_value_t<R>>;

    auto pool = detail::make_pool<T>(range);
    ptrdiff_t r = static_cast<ptrdiff_t>(pool.size());

    return detail::PermutationsRange<T>(move(pool), r);
}

template <class T>
auto product(const vector<vector<T>>& pools, ptrdiff_t repeat = 1) {
    return detail::ProductRange<T>(pools, repeat);
}

template <std::ranges::input_range R>
auto product(const R& range) {
    using T = remove_cv_t<std::ranges::range_value_t<R>>;

    vector<vector<T>> pools{detail::make_pool<T>(range)};
    return detail::ProductRange<T>(move(pools), 1);
}

template <std::ranges::input_range R>
auto product(const R& range, ptrdiff_t repeat) {
    using T = remove_cv_t<std::ranges::range_value_t<R>>;

    vector<vector<T>> pools{detail::make_pool<T>(range)};
    return detail::ProductRange<T>(move(pools), repeat);
}

template <std::ranges::input_range R1, std::ranges::input_range R2, std::ranges::input_range... Rs>
auto product(const R1& range1, const R2& range2, const Rs&... ranges) {
    using T = common_type_t<remove_cv_t<std::ranges::range_value_t<R1>>, remove_cv_t<std::ranges::range_value_t<R2>>, remove_cv_t<std::ranges::range_value_t<Rs>>...>;

    vector<vector<T>> pools;
    pools.reserve(2 + sizeof...(Rs));

    pools.push_back(detail::make_pool<T>(range1));
    pools.push_back(detail::make_pool<T>(range2));
    (pools.push_back(detail::make_pool<T>(ranges)), ...);

    return detail::ProductRange<T>(move(pools), 1);
}

template <std::ranges::input_range R>
auto combinations_with_replacement(const R& range, ptrdiff_t r) {
    using T = remove_cv_t<std::ranges::range_value_t<R>>;
    return detail::CombinationsWithReplacementRange<T>(detail::make_pool<T>(range), r);
}

} // namespace itertools