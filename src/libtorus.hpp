#ifndef _TORUS_
#define _TORUS_

/**
 * @brief A Torus shapped DataType that loops in both dimensions
 * @details This data type is functionally similar to a linked list, just
 *  overengineered. This functions as a multi-deminsional doubly linked list
 *  that is connected on both deminsions forming what is basically a Torus
 *  in memory.
 *
 * @tparam T The datatype the Torus should store, like a Linked List
 */

template <class T>
class Torus {

    // The data node for each point in the Torus
    struct Node {
        T data;
        bool initialized;

        unsigned int x;
        unsigned int y;

        Node* left;
        Node* right;
        Node* up;
        Node* down;
    };

    // Tracks the position inside the Torus, and tracks the size of the Torus
    Node* cursor;
    unsigned int width;
    unsigned int height;
    int* references;


    /**
     * @brief Clears all memory for the Torus
     * @details This will seek to the beginning of the torus, then iterate
     * over each point, clearing the memory, until the Torus is empty. This
     * should be reserved for anything that overwrites the object, or for
     * when the object is being discarded.
     */
    void clear() {
        Node *last;

        for (unsigned int col = 0; col < this->width; col++) {
            for (unsigned int row = 0; row < this->height; row++) {
                last = this->cursor;

                if (row < this->height - 1) {
                    this->cursor = this->cursor->down;
                } else {
                    this->cursor = this->cursor->right;
                }

                delete last;
            }
        }
    }


public:

    /**
     * @brief Construct a new Torus object with a default shape of 1,1
     */
    Torus() : Torus(1,1) {}

    /**
     * @brief Construct a new Torus object
     *
     * @param width The width, or X dimension, of the Torus
     * @param height The height, or Y dimension, of the Torus
     */
    Torus(unsigned int width, unsigned int height) {
        Node* temp;
        Node* last;
        Node* last_col;
        Node* first;
        Node* first_row;

        for (unsigned int col = 0; col < width; col++) {
            for (unsigned int row = 0; row < height; row++) {

                temp = new Node();
                temp->x = col;
                temp->y = row;

                if (col == 0 && row == 0)
                    first = temp;

                // Hold the first item in the row so we can wrap
                // the column into a tube
                if (row == 0)
                    first_row = temp;

                // Link current column against last
                if (col > 0) {
                    last_col->right = temp;
                    temp->left = last_col;
                    last_col = last_col->down;
                }

                // Connect current Node against last Node
                if (!(row == 0)) {
                    last->down = temp;
                    temp->up = last;
                }

                last = temp;
            }

            // Fold the column against itself
            first_row->up = last;
            last->down = first_row;

            last = nullptr;
            last_col = first_row;
        }

        this->cursor = first;  // Setup the cursor
        this->width = width;
        this->height = height;
        this->references = new int(1);

        // Wrap what is effectively a cylinder into itself now
        // to create the torus
        for (unsigned int row = 0; row < height; row++) {
            first->left = last_col;
            last_col->right = first;
            first = first->down;
            last_col = last_col->down;
        }
    }

    /**
     * @brief Constructs a new Torus from an existing Torus
     * @details This is a shallow copy, the individual nodes that form
     *  the Torus will be shared between the two Tori
     * @param other The other Torus to copy from
     */
    Torus(Torus &other) :
        width(other.width), height(other.height), cursor(other.cursor) {
            this->references = other.references;
            (*this->references)++;
        };

    /**
     * @brief Destroy the Torus object
     * @details Will destroy the object, unless the underlying Torus is
     *  shared between objects, then the reference is just decremented.
     */
    ~Torus() {
        if (*this->references == 1)
            this->clear();
        else
            (*this->references)--;
    }

    /**
     * @brief Makes two copies of the same Torus share the same location
     * @details This is a shallow copy if the underlying Tori are the same, or
     *  if there is more than one reference to the current Torus. Otherwise,
     *  this will clear the existing Torus and then perform the shallow copy.
     *
     * @param other Another torus to set this object too
     * @return Torus
     */
    Torus operator=(const Torus<T> &other) {
        if (this == other) {
            this->cursor = other.cursor;
            return this;
        }

        if (*this->references == 1) {
            clear();
        }

        this->references = other.references;
        this->height = other.height;
        this->width = other.width;
        this->cursor = other.cursor;
        return this;
    }

    /**
     * @brief Compares the existing Torus to another Torus
     *
     * @param other The other Torus
     * @return true If the two Torus are the same
     * @return false If the two Torus are different
     */
    bool operator==(const Torus<T> &other) const {
        unsigned int saved_x, saved_y;
        bool same_address;

        saved_x = this->cursor->x;
        saved_y = this->cursor->y;

        if (!(this->width == other.width && this->height == other.height)) {
            return false;
        }

        this->seek(other.get_x(), other.get_y());
        same_address = this->cursor == other.cursor;
        this->seek(saved_x, saved_y);

        return same_address;
    }

    /**
     * @brief Seeks to the desired position in the Torus
     *
     * @param x X cordinate to seek to
     * @param y Y cordinate to seek to
     */
    void seek(unsigned int x, unsigned int y) {
        // TODO: Actually calculate the shortest distance and take that
        // route instead

        int x_move_down;
        bool x_loops;

        int y_move_right;
        bool y_loops;

        if (this->width < x)
            throw "X position is larger than the torus";
        if (this->height < y)
            throw "Y position is larger than the torus";

        x_loops = !(this->cursor->x < x);
        y_loops = !(this->cursor->y < y);

        x_move_down = !x_loops * (x - this->cursor->x)
            + x_loops * (this->height - this->cursor->x + x);
        y_move_right = !y_loops * (y - this->cursor->y)
            + y_loops * (this->width - this->cursor->y + y);


        // Actually move the cursor
        for (int right = 0; right < y_move_right; right++)
            this->move_right();

        for (int down = 0; down < x_move_down; down++)
            this->move_down();
    }

    /**
     * @brief Determines if the current node actually contains data
     *
     * @return true If the current node contains memory
     * @return false If the current node does not contain memory
     */
    bool is_valid() const { return this->cursor->initialized; }

    /**
     * @brief Get the data stored in the current node
     *
     * @return T current data
     */
    T get_data() const { return this->cursor->data; }

    /**
     * @brief Sets the memory at the current node as invalid
     */
    void clear_data() { this->cursor->initialized = false; }

    /**
     * @brief Set the data for the current node
     *
     * @param data
     */
    void set_data(T &data) {
        this->cursor->data = data;
        this->cursor->initialized = true;
    }

    /**
     * @brief Moves the cursor left
     */
    void move_left() {this->cursor = this->cursor->left;}

    /**
     * @brief Moves the cursor multiple steps to the left
     * @param steps How many steps left to move the cursor
     */
    void move_left(unsigned int steps) {
        for (int i=0; i < steps; i++)
            this->move_left();
    }

    /**
     * @brief Moves the cursor right
     */
    void move_right() {this->cursor = this->cursor->right;}

    /**
     * @brief Moves the cursor multiple steps to the right
     * @param steps How many steps right to move the cursor
     */
    void move_right(unsigned int steps) {
        for (int i=0; i < steps; i++)
            this->move_right();
    }

    /**
     * @brief Moves the cursor up
     */
    void move_up() {this->cursor = this->cursor->up;}

    /**
     * @brief Moves the cursor multiple steps up
     * @param steps How many steps to move the cursor
     */
    void move_up(unsigned int steps) {
        for (int i=0; i < steps; i++)
            this->move_up();
    }

    /**
     * @brief Moves the cursor down
     */
    void move_down() {this->cursor = this->cursor->down;}

    /**
     * @brief Moves the cursor multiple steps down
     * @param steps How many steps to move the cursor
     */
    void move_down(unsigned int steps) {
        for (int i=0; i < steps; i++)
            this->move_down();
    }

    /**
     * @brief Get the X position of the cursor
     * @return unsigned int
     */
    unsigned int get_x() const { return this->cursor->x; }

    /**
     * @brief Get the Y position of the cursor
     * @return unsigned int
     */
    unsigned int get_y() const { return this->cursor->y; }

    /**
     * @brief Get the width of the Torus Grid
     * @return unsigned int
     */
    unsigned int get_width() const { return this->width; }

    /**
     * @brief Get the height of the Torus Grid
     * @return unsigned int
     */
     unsigned int get_height() const { return this->height; }

};

#endif
