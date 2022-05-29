export interface Recipe {
  id: number;
  name: string;
  image: string;
  description: string;
  products?: Product[];
}

export interface Product {
  name: string;
  mass: number;
}
