# Coding guidelines #

### Function bodies ###
    int CamelCaseFunction(void *something) {
        int myInt;
        int myInt2;
        
        myInt = 0;
        myInt2 = 0;
        ...
    }

### Enums ###
    enum COLOR_T {
        RED,
        BLUE,
        GREEN
    };
    
    // Names can also be prefixed if needed
    
    enum MOVEMENT {
        MOVEMENT_RUN,
        MOVEMENT_WALK,
        MOVEMENT_JUMP
    };