import { Component, Input } from '@angular/core';
import { Recipe } from '../recipe.interface';
import { RECIPES } from '../recipe.data';

@Component({
  selector: 'app-recipe-detail',
  templateUrl: './recipe-detail.component.html',
  styleUrls: ['./recipe-detail.component.css'],
})
export class RecipeDetailComponent {
  @Input() recipe: Recipe;

  constructor() {
    this.recipe = RECIPES[0];
  }
}
