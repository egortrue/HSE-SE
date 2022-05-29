import { Component, Input } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Location } from '@angular/common';

import { Recipe } from '../recipe.interface';
import { RECIPES } from '../recipe.data';
import { CartService } from '../../cart/cart.service';

@Component({
  selector: 'app-recipe-detail',
  templateUrl: './recipe-detail.component.html',
  styleUrls: ['./recipe-detail.component.css'],
})
export class RecipeDetailComponent {
  recipe: Recipe;

  constructor(
    private route: ActivatedRoute,
    private location: Location,
    public cartService: CartService
  ) {
    let id = Number(this.route.snapshot.paramMap.get('id'));
    if (isNaN(id) || id < 0 || RECIPES.length <= id) {
      id = 0;
      this.location.go('/recipes/0');
    }
    this.recipe = RECIPES[id];
  }
}
