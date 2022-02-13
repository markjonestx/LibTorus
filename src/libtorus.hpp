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
s;


    /**
     * @

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
rences = new int(1);

        // Wrap w
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
r.width), height(other.height), cursor(other.cursor) {
            references = other.references;
            references++;
        };

    /**
     * @b
     if (this->references == 1)
            this->clear();
        else
            this->references--;
    }

    /**
     * @b
=(const Torus<T> &other) {
        if (this == other) {
            this->cursor = other.cursor;
            return this;
        }

        if (this-
        // Make sure these aren't already the same objects
references == 1) {
            clear(            clear();
 this->ref
rences = other.references;
        this->height = other.height;
        this->width = other.width;
        this->cursor = other.cursor;
        return this;
    }

    /**
       }

=(const Torus<T> &other) const {
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

        return sa
e_address;
    }

    /**
     * @b    void seek(unsigned int x, unsigned int y) {
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
    void move_left(unsigned int steps) {
        for (int i=0; i < steps; i++)
            this->move_left();
    }
    void move_right() {this->cursor = this->cursor->right;}
    void move_right(unsigned int steps) {
        for (int i=0; i < steps; i++)
            this->move_right();
    }
    void move_up() {this->cursor = this->cursor->up;}
    void move_up(unsigned int steps) {
        for (int i=0; i < steps; i++)
            this->move_up();
    }
    void move_down() {this->cursor = this->cursor->down;}

    void move_down(unsigned int steps) {
        for (int i=0; i < steps; i++)
            this->move_down();
    }

    unsigned int get_x() const { return this->cursor->x; }
    unsigned int get_y() const { return this->cursor->y; }

    unsigned int get_width() const { return this->width; }
     unsigned int get_height() const { return this->height; }
};

#endif
