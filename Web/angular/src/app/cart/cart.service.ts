import { isNull } from '@angular/compiler/src/output/output_ast';
import { Injectable } from '@angular/core';
import { Product } from '../recipe/recipe.interface';

@Injectable({
  providedIn: 'root',
})
export class CartService {
  products: Product[] = [];

  constructor() {
    let raw = localStorage.getItem('products');
    console.log(raw);
    if (raw != null) this.products = JSON.parse(raw);
  }

  add(product: Product) {
    this.products = [product, ...this.products];
    this.updateStorage();
  }

  remove(index: number) {
    this.products.splice(index, 1);
    this.updateStorage();
  }

  getProducts(): Product[] {
    return this.products;
  }

  updateStorage() {
    localStorage.setItem('products', JSON.stringify(this.products));
  }
}
