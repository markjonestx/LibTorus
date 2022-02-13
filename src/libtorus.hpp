#ifndef _TORUS_
#define _TORUS_

/**
 * @brief A
 *
 * @tparam T
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

    // Tracks the position inside the Torus, and tracks boundaries
    Node* cursor;
    unsigned int width;
    unsigned int height;


    /**
     * @class Torus
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

    Torus() : Torus(1,1) {}
    Torus(unsigned int width, unsigned int height) {
        Node* temp;
        Node* last;
        Node* last_col;
        Node* first;
        Node* first_row;

        // Initialize one column at a time
        for (unsigned int col = 0; col < width; col++) {
            for (unsigned int row = 0; row < height; row++) {

                temp = new Node();
                temp->x = col;
                temp->y = row;

                // Grab the first values
                if (col == 0 && row == 0)
                    first = temp;

                // Hold the first item in the row
                // so we can wrap the column into a tube
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

                // Mark current Node as last Node
                last = temp;
            }

            // Fold the column against itself
            first_row->up = last;
            last->down = first_row;

            // Prep for next series of iterations
            last = nullptr;
            last_col = first_row;
        }

        this->cursor = first;  // Setup the cursor
        this->width = width;
        this->height = height;

        // Wrap what is effectively a cylinder into itself now
        // to create the torus
        for (unsigned int row = 0; row < height; row++) {
            first->left = last_col;
            last_col->right = first;
            first = first->down;
            last_col = last_col->down;
        }
    }


    Torus(Torus &other) :
        width(other.width), height(other.height), cursor(other.cursor) {};


    ~Torus() { this->clear(); }


    Torus operator=(const Torus &other) {
        seek(other.x, other.y);

        // Make sure these aren't already the same objects
        if (this->cursor != other.cursor)
            clear();

        this->cursor = cursor;
        this->width = width;
        this->height = height;
    }


    void seek(unsigned int x, unsigned int y) {
        // 50/50 shot this is efficient.

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


    bool is_valid() const { return this->cursor->initialized; }
    T get_data() const { return this->cursor->data; }
    void clear_data() { this->cursor->initialized = false; }

    void set_data(T &data) {
        this->cursor->data = data;
        this->cursor->initialized = true;
    }


    void move_left() {this->cursor = this->cursor->left;}
    void move_right() {this->cursor = this->cursor->right;}
    void move_up() {this->cursor = this->cursor->up;}
    void move_down() {this->cursor = this->cursor->down;}


    unsigned int get_x() const { return this->cursor->x; }
    unsigned int get_y() const { return this->cursor->y; }

};

#endif
