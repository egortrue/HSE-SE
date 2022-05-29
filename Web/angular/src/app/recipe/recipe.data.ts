import { Recipe } from './recipe.interface';

export const RECIPES: Recipe[] = [
  {
    id: 0,
    name: 'Borscht',
    image: 'assets/borscht.jpeg',
    description: `Borscht is a type of sour red beet soup.
                  It is the main first meal in Ukrainian cuisine.
                  It is also oftenly eaten in Eastern European countries, 
                  such as Russia, Romania, Poland, Belarus, Moldova and
                  Lithuania. It contains red beets, sausage, onion,
                  potatoes and cabbage. Borshch soup is usually eaten
                  with a piece of black bread.It is also eaten in central
                  Asian countries like Kazakhstan and Turkmenistan.`,
    products: [
      { name: 'beetroots', mass: 100 },
      { name: 'carrots', mass: 100 },
      { name: 'onions', mass: 20 },
      { name: 'potatoes', mass: 100 },
      { name: 'black pepper', mass: 5 },
    ],
  },
  {
    id: 1,
    name: 'Hamburger',
    image: 'assets/hamburger.jpeg',
    description: `A hamburger or burger is an American fast food.
                  It is a type of sandwich with a patty of cooked ground meat
                  between the two halves of a bun. Tomatoes, onions, cheese,
                  salad and dips may also be added. The hamburger may be eaten
                  without a knife and fork, so it is a fast food which can
                  be eaten anywhere.`,
    products: [
      { name: 'bread', mass: 100 },
      { name: 'mayonnaise', mass: 50 },
      { name: 'onions', mass: 30 },
      { name: 'beef', mass: 150 },
      { name: 'black pepper', mass: 5 },
    ],
  },
  {
    id: 2,
    name: 'Barbecue',
    image: 'assets/barbecue.jpg',
    description: `Barbecue (or Barbeque, BBQ) is a special type of grill.
                  It is also a way to prepare meat which is then cooked with
                  a barbecue sauce. It is especially popular in Australia
                  and in certain parts of the United States.
                  Barbecue cooking is often done very slowly. It is not as
                  hot as grilling. Some meats must be cooked slowly
                  to be tender, and easy to chew. Sometimes, meat may
                  be slowly cooked for 8 to 24 hours on a barbecue.`,
    products: [
      { name: 'beef', mass: 500 },
      { name: 'ketchup', mass: 50 },
      { name: 'cider vinegar', mass: 50 },
      { name: 'brown sugar', mass: 50 },
      { name: 'black pepper', mass: 10 },
    ],
  },

  {
    id: 3,
    name: 'Pasta',
    image: 'assets/pasta.jpeg',
    description: `Pasta is a food made from flour, starch and water.
                  It is usually cooked in hot water before being eaten.
                  Pasta has been very popular in Italy and has also been
                  eaten in many parts of Asia for a long time. It is
                  also the national dish of Italy, invented in 1986`,
    products: [
      { name: 'macaroni', mass: 200 },
      { name: 'olive oil', mass: 50 },
      { name: 'ketchup', mass: 20 },
      { name: 'salt', mass: 5 },
    ],
  },
  {
    id: 4,
    name: 'Pancakes',
    image: 'assets/pancakes.jpg',
    description: `A pancake (or hot-cake, griddlecake, or flapjack) is a
                  flat cake, often thin and round, prepared from a
                  starch-based batter that may contain eggs, milk
                  and butter and cooked on a hot surface such as
                  a griddle or frying pan, often frying with oil
                  or butter. It is a type of batter bread.
                  Archaeological evidence suggests that pancakes
                  were probably eaten in prehistoric societies.`,
    products: [
      { name: 'flour', mass: 100 },
      { name: 'white sugar', mass: 15 },
      { name: 'baking powder', mass: 20 },
      { name: 'milk', mass: 100 },
      { name: 'vegetable oil', mass: 25 },
      { name: 'beaten egg', mass: 100 },
    ],
  },
];
