import { Component } from '@angular/core';
import { NgForm } from '@angular/forms';
import { CartService } from './cart.service';
import { Product } from '../recipe/recipe.interface';

@Component({
  selector: 'app-cart',
  templateUrl: './cart.component.html',
  styleUrls: ['./cart.component.css'],
})
export class CartComponent {
  constructor(public cartService: CartService) {}

  onSubmit(form: NgForm) {
    const value = form.value;
    const name: string = form.value.name;
    const mass: number = form.value.amount;
    if (name.length <= 0 || mass <= 0) {
      form.reset();
      return;
    }

    const newProduct: Product = { name: name, mass: mass };
    this.cartService.add(newProduct);
    form.reset();
  }
}
